#include "demos.h"

#include <iostream>

using namespace manta;

void manta_demo::stressSpidersDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
    // Top-level parameters
    constexpr bool LENS_SIMULATION = true;
    constexpr bool USE_ACCELERATION_STRUCTURE = true;
    constexpr bool DETERMINISTIC_SEED_MODE = false;
    constexpr bool TRACE_SINGLE_PIXEL = false;
    constexpr bool ENABLE_FRAUNHOFER_DIFFRACTION = false;

    Scene scene;
    RayTracer rayTracer;
    rayTracer.setMaterialLibrary(new MaterialLibrary);

    // Load all object files
    ObjFileLoader stressSpidersObj;
    bool result = stressSpidersObj.loadObjFile(MODEL_PATH "stress_spiders.obj");

    if (!result) {
        std::cout << "Could not open geometry file" << std::endl;

        stressSpidersObj.destroy();

        return;
    }

    // Create all materials
    LambertianBRDF lambert;

    PhongDistribution spiderCoating;
    spiderCoating.setPower((math::real)8192);
    BilayerBRDF spiderBSDF;
    spiderBSDF.setCoatingDistribution(&spiderCoating);
    spiderBSDF.setDiffuse(getColor(0xf1, 0xc4, 0x0f));
    spiderBSDF.setSpecularAtNormal(math::loadVector(0.05f, 0.05f, 0.05f));
    SimpleBSDFMaterial *spiderMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    spiderMaterial->setName("Equipment");
    spiderMaterial->setBSDF(new BSDF(&spiderBSDF));
    spiderMaterial->setEmission(math::constants::Zero);

    PhongDistribution groundCoating;
    groundCoating.setPower((math::real)64);
    BilayerBRDF groundBSDF;
    groundBSDF.setCoatingDistribution(&groundCoating);
    groundBSDF.setDiffuse(getColor(0xFF, 0xFF, 0xFF));
    groundBSDF.setSpecularAtNormal(math::loadVector(0.1f, 0.1f, 0.1f));
    SimpleBSDFMaterial *groundMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    groundMaterial->setName("Ground");
    groundMaterial->setBSDF(new BSDF(&groundBSDF));
    groundMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial outdoorTopLightMaterial;
    outdoorTopLightMaterial.setEmission(math::loadVector(5.f, 5.f, 5.f));
    //outdoorTopLightMaterial.setReflectance(math::constants::Zero);

    // Create all scene geometry
    Mesh stressSpiders;
    stressSpiders.loadObjFileData(&stressSpidersObj);
    stressSpiders.bindMaterialLibrary(rayTracer.getMaterialLibrary(), spiderMaterial->getIndex());
    stressSpiders.setFastIntersectEnabled(false);
    stressSpidersObj.destroy();

    KDTree kdtree;
    kdtree.configure(100.0f, math::constants::Zero);
    kdtree.analyzeWithProgress(&stressSpiders, 4);

    SpherePrimitive outdoorTopLightGeometry;
    outdoorTopLightGeometry.setRadius(10.f);
    outdoorTopLightGeometry.setPosition(math::loadVector(19.45842f, 12.42560f, -13.78918f));

    // Create scene objects
    SceneObject *stressSpidersObject = scene.createSceneObject();
    if (USE_ACCELERATION_STRUCTURE) {
        stressSpidersObject->setGeometry(&kdtree);
    }
    else {
        stressSpidersObject->setGeometry(&stressSpiders);
    }
    stressSpidersObject->setDefaultMaterial(spiderMaterial);
    stressSpidersObject->setName("StressSpiders");

    SceneObject *outdoorTopLightObject = scene.createSceneObject();
    outdoorTopLightObject->setGeometry(&outdoorTopLightGeometry);
    outdoorTopLightObject->setDefaultMaterial(&outdoorTopLightMaterial);
    outdoorTopLightObject->setName("MainLamp");

    math::Vector cameraPos = math::loadVector(7.32725f, 2.69770f, 10.34150f);
    math::Vector target = math::loadVector(0.0f, 1.3f, 0.0f);

    math::Vector up = math::loadVector(0.0f, 1.0f, 0.0f);
    math::Vector dir = math::normalize(math::sub(target, cameraPos));
    up = math::cross(math::cross(dir, up), dir);
    up = math::normalize(up);

    // Create the camera
    CameraRayEmitterGroup *group;
    manta::SimpleLens lens;
    manta::SquareAperture squareAperture;
    lens.setAperture(&squareAperture);
    lens.initialize();
    lens.setPosition(cameraPos);
    lens.setDirection(dir);
    lens.setUp(up);
    lens.setRadius(1.0);
    lens.setSensorResolutionX(resolutionX);
    lens.setSensorResolutionY(resolutionY);
    lens.setSensorHeight(44.0f);
    lens.setSensorWidth(44.0f * (resolutionX / (math::real)resolutionY));
    lens.update();

    RandomSampler sampler;

    if (!LENS_SIMULATION) {
        StandardCameraRayEmitterGroup *camera = new StandardCameraRayEmitterGroup;
        camera->setSampler(&sampler);
        camera->setDirection(dir);
        camera->setPosition(cameraPos);
        camera->setUp(up);
        camera->setPlaneDistance(1.0f);
        camera->setPlaneHeight(0.7f);
        camera->setResolutionX(resolutionX);
        camera->setResolutionY(resolutionY);

        group = camera;
    }
    else {
        math::real lensHeight = 1.0f;
        math::real focusDistance = 11.0f;

        Aperture *aperture = lens.getAperture();
        aperture->setRadius((math::real)0.05f);
        lens.setFocus(focusDistance);

        LensCameraRayEmitterGroup *camera = new LensCameraRayEmitterGroup;
        camera->setDirection(math::normalize(math::sub(target, cameraPos)));
        camera->setPosition(cameraPos);
        camera->setLens(&lens);
        camera->setResolutionX(resolutionX);
        camera->setResolutionY(resolutionY);
        camera->setSampler(&sampler);

        group = camera;
    }

    // Create the raytracer
    rayTracer.configure(200 * MB, 100 * MB, 12, 100, true);
    rayTracer.setBackgroundColor(getColor(255, 255, 255));
    rayTracer.setDeterministicSeedMode(DETERMINISTIC_SEED_MODE);

    // Output the results to a scene buffer
    ImagePlane sceneBuffer;
    GaussianFilter filter;
    filter.setExtents(math::Vector2(1.5, 1.5));
    filter.configure((math::real)4.0);
    sceneBuffer.setFilter(&filter);

    if (TRACE_SINGLE_PIXEL) {
        rayTracer.tracePixel(819, 199, &scene, group, &sceneBuffer);
    }
    else {
        rayTracer.traceAll(&scene, group, &sceneBuffer);
    }

    std::string fname = createUniqueRenderFilename("stress_spiders_demo", samplesPerPixel);
    std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".jpg";
    std::string fraunFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + "_fraun" + ".jpg";
    std::string convFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + "_conv" + ".jpg";
    std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

    if (ENABLE_FRAUNHOFER_DIFFRACTION) {
        VectorMap2D base;
        base.copy(&sceneBuffer);

        int safeWidth = base.getSafeWidth();

        FraunhoferDiffraction testFraun;
        FraunhoferDiffraction::Settings settings;
        FraunhoferDiffraction::setDefaultSettings(&settings);
        settings.frequencyMultiplier = 1.0;
        settings.maxSamples = 4096;
        settings.textureSamples = 10;

        TextureNode dirtTexture;
        dirtTexture.loadFile(TEXTURE_PATH "dirt_very_soft.png", true);
        dirtTexture.initialize();
        dirtTexture.evaluate();
        const VectorMap2D *dirtTextureMap = dirtTexture.getMainOutput()->getMap();

        CmfTable colorTable;
        Spectrum sourceSpectrum;
        colorTable.loadCsv(CMF_PATH "xyz_cmf_31.csv");
        sourceSpectrum.loadCsv(CMF_PATH "d65_lighting.csv");
        
        squareAperture.setRadius(0.5f);
        testFraun.generate(&squareAperture, dirtTextureMap, safeWidth, 2.0f, &colorTable, &sourceSpectrum, &settings);

        VectorMapWrapperNode fraunNode(testFraun.getDiffractionPattern());
        fraunNode.initialize();
        fraunNode.evaluate();

        ImageOutputNode fraunOutputNode;
        fraunOutputNode.setJpegQuality(95);
        fraunOutputNode.setGammaCorrection(true);
        fraunOutputNode.setOutputFilename(fraunFname);
        fraunOutputNode.setInput(fraunNode.getMainOutput());
        fraunOutputNode.initialize();
        fraunOutputNode.evaluate();
        fraunOutputNode.destroy();

        VectorMapWrapperNode baseNode(&base);
        baseNode.initialize();
        baseNode.evaluate();
        baseNode.destroy();

        ConvolutionNode convNode;
        convNode.setInputs(baseNode.getMainOutput(), fraunNode.getMainOutput());
        convNode.setResize(true);
        convNode.setClip(true);
        convNode.initialize();
        convNode.evaluate();

        testFraun.destroy();
        base.destroy();

        ImageOutputNode outputNode;
        outputNode.setJpegQuality(95);
        outputNode.setGammaCorrection(true);
        outputNode.setOutputFilename(convFname);
        outputNode.setInput(convNode.getMainOutput());
        outputNode.initialize();
        outputNode.evaluate();
        outputNode.destroy();

        colorTable.destroy();
        sourceSpectrum.destroy();
        convNode.destroy();
        dirtTexture.destroy();
    }

    RawFile rawFile;
    rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);

    writeJpeg(imageFname.c_str(), &sceneBuffer, 95);

    sceneBuffer.destroy();
    rayTracer.destroy();

    stressSpiders.destroy();
    kdtree.destroy();

    std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
