#include "demos.h"

#include <iostream>

using namespace manta;

void manta_demo::stockSceneDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
    // Top-level parameters
    constexpr bool USE_ACCELERATION_STRUCTURE = true;
    constexpr bool DETERMINISTIC_SEED_MODE = false;
    constexpr bool TRACE_SINGLE_PIXEL = false;
    constexpr bool LENS_SIMULATION = true;
    constexpr bool ALT_VERSION = false;

    RayTracer rayTracer;
    rayTracer.setMaterialLibrary(new MaterialLibrary);

    Scene scene;

    // Load all textures
    TextureNode fingerprintTexture;
    fingerprintTexture.loadFile(TEXTURE_PATH "samsung_a8/fingerprints_roughness_map.png", false);
    fingerprintTexture.initialize();
    fingerprintTexture.evaluate();

    TextureNode metalTexture;
    metalTexture.loadFile(TEXTURE_PATH "stock-scene/metal.jpg", true);
    metalTexture.initialize();
    metalTexture.evaluate();

    TextureNode woodTexture;
    woodTexture.loadFile(TEXTURE_PATH "light_wood.jpg", true);
    woodTexture.initialize();
    woodTexture.evaluate();

    TextureNode eraserTexture;
    eraserTexture.loadFile(TEXTURE_PATH "stock-scene/eraser.jpg", true);
    eraserTexture.initialize();
    eraserTexture.evaluate();

    TextureNode floorTexture;

    if (ALT_VERSION) {
        floorTexture.loadFile(TEXTURE_PATH "stock-scene/floor_alt.png", true);
    }
    else {
        floorTexture.loadFile(TEXTURE_PATH "stock-scene/floor.png", true);
    }
    floorTexture.initialize();
    floorTexture.evaluate();

    // Load all object files
    ObjFileLoader stageObj;
    bool result = stageObj.loadObjFile(MODEL_PATH "stock_scene_alt.obj");

    if (!result) {
        std::cout << "Could not open geometry file(s)" << std::endl;
        stageObj.destroy();
        return;
    }

    // Materials ==============================================================

    LambertianBRDF lambert;

    CachedVectorNode constS;
    constS.setValue(math::loadScalar(4.0f));
    constS.initialize();
    PowerNode texturePower;
    texturePower.initialize();
    texturePower.connectInput(fingerprintTexture.getMainOutput(), "__left", nullptr);
    texturePower.connectInput(constS.getPrimaryOutput(), "__right", nullptr);
    RemapNode specularPowerFingerprint(
        math::loadScalar(0.0f),
        math::loadScalar(1.0f),
        texturePower.getPrimaryOutput());

    RemapNode invFingerprint(
        math::loadScalar(1.0f),
        math::loadScalar(0.5f),
        texturePower.getPrimaryOutput());

    // Steel
    PhongDistribution phongSteel;
    phongSteel.setPower(10000.0f);
    phongSteel.setPowerNode(specularPowerFingerprint.getMainOutput());

    PhongDistribution phongShinySteel;
    phongShinySteel.setPower(5000.0f);

    BilayerBRDF steelBSDF;
    steelBSDF.setCoatingDistribution(&phongSteel);
    steelBSDF.setDiffuseNode(metalTexture.getMainOutput());
    steelBSDF.setSpecularAtNormal(math::loadVector(1.0f, 1.0f, 1.0f));
    steelBSDF.setSpecularNode(invFingerprint.getMainOutput());

    MicrofacetBRDF shinySteelBSDF;
    shinySteelBSDF.setDistribution(&phongShinySteel);

    SimpleBSDFMaterial *steelMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    steelMaterial->setName("Steel");
    steelMaterial->setBSDF(new BSDF(&steelBSDF));
    steelMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *shinySteelMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    shinySteelMaterial->setName("ShinySteel");
    shinySteelMaterial->setBSDF(new BSDF(&shinySteelBSDF));
    shinySteelMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *blackSteelMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    blackSteelMaterial->setName("BlackSteel");
    //blackSteelMaterial->setReflectance(getColor(0xC0, 0xC0, 0xC0));
    blackSteelMaterial->setBSDF(new BSDF(&steelBSDF));
    blackSteelMaterial->setEmission(math::constants::Zero);

    // Graphite
    PhongDistribution phongGraphite;
    phongGraphite.setPower(512);

    MicrofacetBRDF graphiteBSDF;
    graphiteBSDF.setDistribution(&phongGraphite);

    SimpleBSDFMaterial *graphiteMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    graphiteMaterial->setName("Graphite");
    //graphiteMaterial->setReflectance(getColor(0x29, 0x29, 0x29));
    graphiteMaterial->setBSDF(new BSDF(&graphiteBSDF));
    graphiteMaterial->setEmission(math::constants::Zero);

    // Pencil paint
    PhongDistribution phongGloss;
    phongGloss.setPower(1024);
    phongGloss.setPowerNode(fingerprintTexture.getMainOutput());

    BilayerBRDF paintBSDF;
    paintBSDF.setCoatingDistribution(&phongGloss);
    if (ALT_VERSION) paintBSDF.setDiffuse(getColor(0x0F, 0x10, 0x40));
    else paintBSDF.setDiffuse(getColor(0xE9, 0xE2, 0xDF));
    paintBSDF.setSpecularAtNormal(math::loadScalar(0.1f));

    SimpleBSDFMaterial *paintMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    paintMaterial->setName("PencilPaint");
    paintMaterial->setBSDF(new BSDF(&paintBSDF));
    paintMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *defaultLambert = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    defaultLambert->setBSDF(new BSDF(&lambert));
    defaultLambert->setName("Default");
    defaultLambert->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *rubber = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    rubber->setBSDF(new BSDF(&lambert));
    rubber->setName("Eraser");
    //rubber->setReflectance(getColor(0xFF, 0xFF, 0xFF));
    //rubber->setReflectanceNode(eraserTexture.getMainOutput());
    rubber->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *wood = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    wood->setBSDF(new BSDF(&lambert));
    wood->setName("Wood");
    //wood->setReflectance(getColor(0xFF, 0xFF, 0xFF));
    //wood->setReflectanceNode(woodTexture.getMainOutput());
    wood->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *floorMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    floorMaterial->setBSDF(new BSDF(&lambert));
    floorMaterial->setName("Floor");
    //floorMaterial->setReflectanceNode(floorTexture.getMainOutput());
    floorMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *mainLight = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    mainLight->setBSDF(nullptr);
    mainLight->setName("Light");
    mainLight->setEmission(math::loadScalar(4.0f));
    //mainLight->setReflectance(math::constants::Zero);

    // Create all scene geometry
    Mesh stage;
    stage.loadObjFileData(&stageObj, 0);
    stage.bindMaterialLibrary(rayTracer.getMaterialLibrary(), defaultLambert->getIndex());

    // Destroy file loaders
    stageObj.destroy();

    KDTree kdtree;

    // Create scene objects
    SceneObject *stageObject = scene.createSceneObject();
    if (USE_ACCELERATION_STRUCTURE) {
        kdtree.configure(100, math::constants::Zero);
        kdtree.analyzeWithProgress(&stage, 4);
        stageObject->setGeometry(&kdtree);
    }
    else {
        stageObject->setGeometry(&stage);
    }
    stageObject->setDefaultMaterial(defaultLambert);
    stageObject->setName("Stage");

    // Create the camera
    math::Vector cameraPos = math::loadVector(0.0f, 52.45f, 0.0f);
    math::Vector target = math::loadVector(0.0f, 0.0f, 0.0f);
    math::Vector up = math::loadVector(0.0f, 0.0f, -1.0f);
    math::real cameraPlaneSize = 0.425f;

    cameraPos = math::loadVector(16.514f, 7.9591f, 18.127f);
    target = math::loadVector(1.0412f, -1.0326f, 0.33937f);
    up = math::loadVector(0.0f, 1.0f, 0.0f);

    math::Vector dir = math::normalize(math::sub(target, cameraPos));
    up = math::cross(math::cross(dir, up), dir);
    up = math::normalize(up);

    // Offset the camera along its axis
    cameraPos = math::sub(cameraPos, math::mul(dir, math::loadScalar(0.0f)));

    // Use a custom resolution
    resolutionX = 1900 * 1;
    resolutionY = 1268 * 1;

    CameraRayEmitterGroup *group;
    RandomSampler sampler;

    manta::SimpleLens lens;
    lens.initialize();
    lens.setPosition(cameraPos);
    lens.setDirection(dir);
    lens.setUp(up);
    lens.setRadius(1.0f);
    lens.setSensorResolutionX(resolutionX);
    lens.setSensorResolutionY(resolutionY);
    lens.setSensorHeight(8.0f);
    lens.setSensorWidth(8.0f * (resolutionX / (math::real)resolutionY));
    lens.configure();
    lens.update();

    if (!LENS_SIMULATION) {
        StandardCameraRayEmitterGroup *camera = new StandardCameraRayEmitterGroup;
        camera->setSampler(&sampler);
        camera->setDirection(dir);
        camera->setPosition(cameraPos);
        camera->setUp(up);
        camera->setPlaneDistance(1.0f);
        camera->setPlaneHeight(cameraPlaneSize);
        camera->setResolutionX(resolutionX);
        camera->setResolutionY(resolutionY);
        group = camera;
    }
    else {
        math::real lensHeight = 1.0f;
        math::real focusDistance = 22.0f;

        Aperture *aperture = lens.getAperture();
        aperture->setRadius((math::real)0.3f); // Relatively wide aperture
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
    rayTracer.configure(200 * MB, 50 * MB, 12, 100, true);
    rayTracer.setBackgroundColor(getColor(0, 0, 0));
    rayTracer.setDeterministicSeedMode(DETERMINISTIC_SEED_MODE);
    rayTracer.setPathRecordingOutputDirectory("../../workspace/diagnostics/");

    // Output the results to a scene buffer
    ImagePlane sceneBuffer;
    GaussianFilter filter;
    filter.setExtents(math::Vector2(1.5, 1.5));
    filter.configure((math::real)4.0);
    sceneBuffer.setFilter(&filter);

    if (TRACE_SINGLE_PIXEL) {
        rayTracer.tracePixel(1522, 674, &scene, group, &sceneBuffer);
    }
    else {
        rayTracer.traceAll(&scene, group, &sceneBuffer);
    }

    // Clean up the camera
    delete group;

    std::string fname = createUniqueRenderFilename("stock_scene", samplesPerPixel);
    std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".jpg";
    std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

    RawFile rawFile;
    rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);

    writeJpeg(imageFname.c_str(), &sceneBuffer, 95);

    sceneBuffer.destroy();
    rayTracer.destroy();
    fingerprintTexture.destroy();
    metalTexture.destroy();
    woodTexture.destroy();
    eraserTexture.destroy();
    floorTexture.destroy();
    stage.destroy();
    kdtree.destroy();

    std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
