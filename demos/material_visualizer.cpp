#include "demos.h"

#include <iostream>

using namespace manta;

void manta_demo::materialVisualizer(int samplesPerPixel, int resolutionX, int resolutionY) {
    // Top-level parameters
    constexpr bool USE_ACCELERATION_STRUCTURE = true;
    constexpr bool DETERMINISTIC_SEED_MODE = false;
    constexpr bool TRACE_SINGLE_PIXEL = false;
    constexpr const char *MATERIAL = "Steel2";

    RayTracer rayTracer;
    rayTracer.setMaterialLibrary(new MaterialLibrary);

    Scene scene;

    // Load all textures
    TextureNode checkerboardTexture;
    checkerboardTexture.loadFile(TEXTURE_PATH "material_visualizer/checkerboard.png", true);
    checkerboardTexture.initialize();
    checkerboardTexture.evaluate();

    // Load all object files
    ObjFileLoader stageObj;
    bool result = stageObj.loadObjFile(MODEL_PATH "material_visualizer.obj");

    if (!result) {
        std::cout << "Could not open geometry file(s)" << std::endl;
        stageObj.destroy();
        return;
    }

    // Materials ==============================================================

    LambertianBRDF lambert;

    // Glass
    PhongDistribution phongGlass;
    phongGlass.setPower(50000);

    DielectricMediaInterface fresnel;
    fresnel.setIorIncident((math::real)1.0);
    fresnel.setIorTransmitted((math::real)1.45);
    MicrofacetGlassBSDF simpleGlassBSDF;
    simpleGlassBSDF.setDistribution(&phongGlass);
    simpleGlassBSDF.setMediaInterface(&fresnel);

    SimpleBSDFMaterial *glassMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    glassMaterial->setName("Phong_Glass");
    glassMaterial->setReflectance(getColor(255, 255, 255));
    glassMaterial->setBSDF(new BSDF(&simpleGlassBSDF));

    // GGX Glass
    GgxDistribution ggxGlass;
    ggxGlass.setWidth(0.0001f);

    MicrofacetGlassBSDF ggxGlassBSDF;
    ggxGlassBSDF.setDistribution(&ggxGlass);
    ggxGlassBSDF.setMediaInterface(&fresnel);

    SimpleBSDFMaterial *ggxGlassMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    ggxGlassMaterial->setName("GGX_Glass");
    ggxGlassMaterial->setReflectance(getColor(255, 255, 255));
    ggxGlassMaterial->setBSDF(new BSDF(&ggxGlassBSDF));

    // Simple Wood
    TextureNode texture;
    texture.loadFile(TEXTURE_PATH "/dark_wood.jpg", true);
    texture.initialize();
    texture.evaluate();

    TextureNode woodRoughness;
    woodRoughness.loadFile(TEXTURE_PATH "/wood_roughness.jpg", 1.0f);
    woodRoughness.initialize();
    woodRoughness.evaluate();

    PhongDistribution woodCoating;
    woodCoating.setPower(1000);
    woodCoating.setPowerNode(woodRoughness.getMainOutput());
    woodCoating.setMinMapPower(2);

    BilayerBRDF simpleWoodBSDF;
    simpleWoodBSDF.setCoatingDistribution(woodCoating.getMainOutput());
    simpleWoodBSDF.setDiffuseNode(texture.getMainOutput());
    simpleWoodBSDF.setSpecularAtNormal(math::loadVector(0.0, 0.0, 0.0));

    SimpleBSDFMaterial *paintMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    paintMaterial->setBSDF(new BSDF(&simpleWoodBSDF));
    paintMaterial->setName("SimpleDarkWood");
    paintMaterial->setEmission(math::constants::Zero);
    
    // Steel
    PhongDistribution phongSteel;
    phongSteel.setPower(1024);

    MicrofacetBRDF steelBSDF;
    steelBSDF.setDistribution(&phongSteel);

    SimpleBSDFMaterial *steelMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    steelMaterial->setName("Steel");
    steelMaterial->setReflectance(getColor(255, 255, 255));
    steelMaterial->setBSDF(new BSDF(&steelBSDF));
    steelMaterial->setEmission(math::constants::Zero);

    // Steel 2
    TextureNode fingerprintTexture;
    fingerprintTexture.loadFile(TEXTURE_PATH "samsung_a8/fingerprints_roughness_map.png", false);
    fingerprintTexture.initialize();
    fingerprintTexture.evaluate();

    TextureNode metalTexture;
    metalTexture.loadFile(TEXTURE_PATH "stock-scene/metal.jpg", true);
    metalTexture.initialize();
    metalTexture.evaluate();

    CachedVectorNode constS;
    constS.initialize();
    constS.setValue(math::loadScalar(4.0f));
    PowerNode fingerprintPower;
    fingerprintPower.initialize();
    fingerprintPower.connectInput(fingerprintTexture.getMainOutput(), "__right", nullptr);
    fingerprintPower.connectInput(constS.getPrimaryOutput(), "__left", nullptr);
    RemapNode specularPowerFingerprint(
        math::loadScalar(0.0f),
        math::loadScalar(1.0f),
        fingerprintPower.getPrimaryOutput());

    RemapNode invFingerprint(
        math::loadScalar(1.0f),
        math::loadScalar(0.0f),
        fingerprintTexture.getMainOutput());

    // Steel
    PhongDistribution phongSteel2;
    phongSteel2.setPower(5000);
    phongSteel2.setPowerNode(specularPowerFingerprint.getMainOutput());
    phongSteel2.setMinMapPower(8);

    BilayerBRDF steelBSDF2;
    steelBSDF2.setCoatingDistribution(phongSteel2.getMainOutput());
    steelBSDF2.setDiffuseNode(metalTexture.getMainOutput());
    steelBSDF2.setSpecularAtNormal(math::loadVector(1.0f, 1.0f, 1.0f));
    steelBSDF2.setSpecularNode(invFingerprint.getMainOutput());

    SimpleBSDFMaterial *steel2Material = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    steel2Material->setName("Steel2");
    steel2Material->setBSDF(new BSDF(&steelBSDF2));
    steel2Material->setEmission(math::constants::Zero);

    // GGX Test
    GgxDistribution ggxTest;
    ggxTest.setWidth(0.9f);

    PhongDistribution ggxPhongEquiv;
    ggxPhongEquiv.setPower(10.0f);

    MicrofacetBRDF ggxTestBDSF;
    ggxTestBDSF.setDistribution(&ggxPhongEquiv);

    SimpleBSDFMaterial *ggxTestMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    ggxTestMaterial->setName("GGX_Test");
    ggxTestMaterial->setBSDF(new BSDF(&ggxTestBDSF));
    ggxTestMaterial->setEmission(math::constants::Zero);
    ggxTestMaterial->setReflectance(math::constants::One);

    // ========================================================================

    // Create all scene materials
    SimpleBSDFMaterial *defaultLambert = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    defaultLambert->setBSDF(new BSDF(&lambert));
    defaultLambert->setName("Default");

    SimpleBSDFMaterial *backdropTexture = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    backdropTexture->setBSDF(new BSDF(&lambert));
    backdropTexture->setReflectanceNode(checkerboardTexture.getMainOutput());
    backdropTexture->setEmission(math::constants::Zero);
    backdropTexture->setName("Backdrop");

    SimpleBSDFMaterial *lightMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    lightMaterial->setEmission(math::loadVector(3.0, 3.0, 3.0));
    lightMaterial->setReflectance(math::constants::Zero);
    lightMaterial->setName("Light");

    // Get the material to display
    Material *displayMaterial = rayTracer.getMaterialLibrary()->searchByName(MATERIAL);

    MaterialPointer *displayMaterialReference = rayTracer.getMaterialLibrary()->newMaterial<MaterialPointer>();
    displayMaterialReference->setName("Main");
    displayMaterialReference->setMaterial(displayMaterial);

    // Create all scene geometry
    Mesh stage;
    stage.loadObjFileData(&stageObj, rayTracer.getMaterialLibrary(), defaultLambert->getIndex(), 0);

    // Destroy file loaders
    stageObj.destroy();

    KDTree kdtree;
    kdtree.configure(20, math::constants::Zero);
    kdtree.analyzeWithProgress(&stage, 4);

    // Create scene objects
    SceneObject *stageObject = scene.createSceneObject();
    if (USE_ACCELERATION_STRUCTURE) {
        stageObject->setGeometry(&kdtree);
    }
    else {
        stageObject->setGeometry(&stage);
    }
    stageObject->setDefaultMaterial(defaultLambert);
    stageObject->setName("Stage");

    // Create the camera
    math::Vector cameraPos = math::loadVector(0.f, 2.f, 9.f);
    math::Vector target = math::loadVector(0.f, 2.f, 0.f);
    math::Vector up = math::loadVector(0.0f, 1.0f, 0.0f);
    math::real cameraPlaneSize = 0.55f;

    math::Vector dir = math::normalize(math::sub(target, cameraPos));
    up = math::cross(math::cross(dir, up), dir);
    up = math::normalize(up);

    cameraPos = math::sub(cameraPos, math::mul(dir, math::loadScalar(1.0)));

    CameraRayEmitterGroup *group;
    RandomSampler sampler;
    StandardCameraRayEmitterGroup *camera = new StandardCameraRayEmitterGroup;
    camera->setSampler(&sampler);
    camera->setDirection(dir);
    camera->setPosition(cameraPos);
    camera->setUp(up);
    camera->setPlaneDistance(1.0f);
    camera->setPlaneHeight(cameraPlaneSize);
    camera->setResolutionX(1920 / 2);
    camera->setResolutionY(1080 / 2);
    camera->setSampleCount(samplesPerPixel);
    group = camera;

    // Create the raytracer
    rayTracer.configure(200 * MB, 50 * MB, 12, 100, true);
    rayTracer.setBackgroundColor(getColor(100, 100, 100));
    rayTracer.setDeterministicSeedMode(DETERMINISTIC_SEED_MODE);
    rayTracer.setPathRecordingOutputDirectory("../../workspace/diagnostics/");

    // Output the results to a scene buffer
    ImagePlane sceneBuffer;
    GaussianFilter filter;
    filter.setExtents(math::Vector2(1.5, 1.5));
    filter.configure((math::real)4.0);
    sceneBuffer.setFilter(&filter);

    if (TRACE_SINGLE_PIXEL) {
        rayTracer.tracePixel(369, 462, &scene, group, &sceneBuffer);
    }
    else {
        rayTracer.traceAll(&scene, group, &sceneBuffer);
    }

    std::string fname = createUniqueRenderFilename("material_visualizer", samplesPerPixel);
    std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".jpg";
    std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

    RawFile rawFile;
    rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);

    writeJpeg(imageFname.c_str(), &sceneBuffer, 95);

    texture.destroy();
    woodRoughness.destroy(); 
    fingerprintTexture.destroy();
    metalTexture.destroy();
    checkerboardTexture.destroy();
    sceneBuffer.destroy();
    rayTracer.destroy();
    stage.destroy();
    kdtree.destroy();

    std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
