#include "demos.h"

#include <iostream>

using namespace manta;

void manta_demo::samsungA8Demo(int samplesPerPixel, int resolutionX, int resolutionY) {
    enum SCENE {
        FACE_ON_SCENE,
        UPRIGHT_SCENE,
        BANNER_SCENE
    };

    // Top-level parameters
    constexpr bool USE_ACCELERATION_STRUCTURE = true;
    constexpr bool DETERMINISTIC_SEED_MODE = false;
    constexpr bool TRACE_SINGLE_PIXEL = false;
    constexpr SCENE SCENE = BANNER_SCENE;
    constexpr bool HIGH_RESOLUTION = false;
    constexpr bool ENABLE_SMUDGE = true;

    RayTracer rayTracer;
    rayTracer.setMaterialLibrary(new MaterialLibrary);

    Scene scene;

    // Load all textures
    TextureNode speakerGrillTexture;
    speakerGrillTexture.loadFile(TEXTURE_PATH "samsung_a8/speaker_grill.jpg", true);
    speakerGrillTexture.initialize();
    speakerGrillTexture.evaluate();

    TextureNode backPlateTexture;
    backPlateTexture.loadFile(TEXTURE_PATH "samsung_a8/back_plate.png", true);
    backPlateTexture.initialize();
    backPlateTexture.evaluate();

    TextureNode phoneScreenTexture;
    phoneScreenTexture.loadFile(TEXTURE_PATH "samsung_a8/phone_screen.png", true);
    phoneScreenTexture.initialize();
    phoneScreenTexture.evaluate();

    TextureNode groundRoughness;
    groundRoughness.loadFile(TEXTURE_PATH "chrome_roughness.jpg", false);
    groundRoughness.initialize();
    groundRoughness.evaluate();

    TextureNode smudgeMap;
    smudgeMap.loadFile(TEXTURE_PATH "samsung_a8/fingerprints_roughness_map.png", false);
    smudgeMap.initialize();
    smudgeMap.evaluate();

    // Load all object files
    ObjFileLoader phoneObj;
    bool result;

    if (SCENE == FACE_ON_SCENE) {
        result = phoneObj.loadObjFile(MODEL_PATH "samsung_a8_face_on_scene.obj");
    }
    else if (SCENE == UPRIGHT_SCENE) {
        result = phoneObj.loadObjFile(MODEL_PATH "samsung_a8_upright_scene.obj");
    }
    else if (SCENE == BANNER_SCENE) {
        result = phoneObj.loadObjFile(MODEL_PATH "samsung_a8_banner_scene.obj");
    }

    if (!result) {
        std::cout << "Could not open geometry file(s)" << std::endl;
        phoneObj.destroy();
        return;
    }

    // Create all materials
    LambertianBRDF lambert;

    PhongDistribution phongPhoneCase;
    phongPhoneCase.setPower(1024);
    if (ENABLE_SMUDGE && (SCENE == UPRIGHT_SCENE || SCENE == BANNER_SCENE)) {
        phongPhoneCase.setPowerNode(smudgeMap.getMainOutput());
    }

    PhongDistribution phongGlass;
    phongGlass.setPower(5000);
    if (ENABLE_SMUDGE && (SCENE == UPRIGHT_SCENE || SCENE == BANNER_SCENE)) {
        phongGlass.setPowerNode(smudgeMap.getMainOutput());
    }

    PhongDistribution phongBlackPlastic;
    phongBlackPlastic.setPower(5000);

    PhongDistribution phongBayDoor;
    phongBayDoor.setPower(256);

    PhongDistribution phongBronze;
    phongBronze.setPower(256);

    PhongDistribution phongSteel;
    phongSteel.setPower(1024);

    PhongDistribution phongFloor;
    phongFloor.setPower(256);
    if (ENABLE_SMUDGE && (SCENE == UPRIGHT_SCENE || SCENE == BANNER_SCENE)) {
        phongFloor.setPowerNode(groundRoughness.getMainOutput());
    }

    PhongDistribution mattePlasticPhong;
    mattePlasticPhong.setPower(64);

    PhongDistribution mirrorPhong;
    mirrorPhong.setPower(20000);

    BilayerBRDF phoneCaseBSDF;
    phoneCaseBSDF.setCoatingDistribution(&phongPhoneCase);
    phoneCaseBSDF.setDiffuse(getColor(0x0, 0x0, 0x0));
    phoneCaseBSDF.setSpecularAtNormal(math::loadVector(0.01f, 0.01f, 0.01f));

    BilayerBRDF bayDoorBSDF;
    bayDoorBSDF.setCoatingDistribution(&phongBayDoor);
    bayDoorBSDF.setDiffuse(getColor(0x0, 0x0, 0x0));
    bayDoorBSDF.setSpecularAtNormal(math::loadVector(0.0f, 0.0f, 0.0f));

    BilayerBRDF speakerGrillBSDF;
    speakerGrillBSDF.setCoatingDistribution(&phongBayDoor);
    speakerGrillBSDF.setDiffuseNode(speakerGrillTexture.getMainOutput());
    speakerGrillBSDF.setDiffuse(math::loadVector(0.1f, 0.1f, 0.1f));
    speakerGrillBSDF.setSpecularAtNormal(math::loadVector(0.0f, 0.0f, 0.0f));

    BilayerBRDF blackPlasticBSDF;
    blackPlasticBSDF.setCoatingDistribution(&phongBlackPlastic);
    blackPlasticBSDF.setDiffuse(getColor(0x0, 0x0, 0x0));
    blackPlasticBSDF.setSpecularAtNormal(math::loadVector(0.05f, 0.05f, 0.05f));

    MicrofacetBRDF mattePlasticBSDF;
    mattePlasticBSDF.setDistribution(&mattePlasticPhong);

    BilayerBRDF floorBSDF;
    floorBSDF.setCoatingDistribution(&phongFloor);
    if (SCENE == UPRIGHT_SCENE || SCENE == BANNER_SCENE) {
        floorBSDF.setDiffuse(getColor(0, 0, 0));
        floorBSDF.setSpecularAtNormal(math::loadVector(0.05f, 0.05f, 0.05f));
    }
    else if (SCENE == FACE_ON_SCENE) {
        floorBSDF.setDiffuse(getColor(255, 255, 255));
        floorBSDF.setSpecularAtNormal(math::loadVector(0.0f, 0.0f, 0.0f));
    }

    BilayerBRDF backPlateBSDF;
    backPlateBSDF.setCoatingDistribution(&phongBlackPlastic);
    backPlateBSDF.setDiffuseNode(backPlateTexture.getMainOutput());
    backPlateBSDF.setSpecularAtNormal(math::loadVector(0.0f, 0.0f, 0.0f));

    DielectricMediaInterface fresnel;
    fresnel.setIorIncident((math::real)1.0);
    fresnel.setIorTransmitted((math::real)1.4);
    MicrofacetGlassBSDF simpleGlassBSDF;
    simpleGlassBSDF.setDistribution(&phongGlass);
    simpleGlassBSDF.setMediaInterface(&fresnel);

    DielectricMediaInterface lensFresnel;
    lensFresnel.setIorIncident((math::real)1.0);
    lensFresnel.setIorTransmitted((math::real)1.5);
    MicrofacetGlassBSDF lensGlassBSDF;
    lensGlassBSDF.setDistribution(&phongGlass);
    lensGlassBSDF.setMediaInterface(&lensFresnel);

    MicrofacetBRDF bronzeBSDF;
    bronzeBSDF.setDistribution(&phongBronze);

    MicrofacetBRDF steelBSDF;
    steelBSDF.setDistribution(&phongSteel);

    MicrofacetBRDF imageSensorBSDF;
    imageSensorBSDF.setDistribution(&mattePlasticPhong);

    MicrofacetBRDF mirrorBSDF;
    mirrorBSDF.setDistribution(&mirrorPhong);

    SimpleBSDFMaterial *defaultMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    defaultMaterial->setName("Default");
    defaultMaterial->setBSDF(new BSDF(&lambert));
    defaultMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *phoneCaseMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    phoneCaseMaterial->setName("PhoneCase");
    phoneCaseMaterial->setReflectance(getColor(255, 255, 255));
    phoneCaseMaterial->setReflectanceNode(smudgeMap.getMainOutput());
    phoneCaseMaterial->setBSDF(new BSDF(&phoneCaseBSDF));
    phoneCaseMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *bayDoorMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    bayDoorMaterial->setName("BayDoorMaterial");
    bayDoorMaterial->setReflectance(getColor(255, 255, 255));
    bayDoorMaterial->setBSDF(new BSDF(&bayDoorBSDF));
    bayDoorMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *bronzeMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    bronzeMaterial->setName("Bronze");
    bronzeMaterial->setReflectance(getColor(0xC0, 0xA5, 0x70));
    bronzeMaterial->setBSDF(new BSDF(&bronzeBSDF));
    bronzeMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *steelMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    steelMaterial->setName("Steel");
    steelMaterial->setReflectance(getColor(255, 255, 255));
    steelMaterial->setBSDF(new BSDF(&steelBSDF));
    steelMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *blackPlasticMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    blackPlasticMaterial->setName("BlackPlastic");
    blackPlasticMaterial->setReflectance(getColor(255, 255, 255));
    blackPlasticMaterial->setBSDF(new BSDF(&blackPlasticBSDF));
    blackPlasticMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *blackMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    blackMaterial->setName("Black");
    blackMaterial->setReflectance(getColor(0, 0, 0));
    blackMaterial->setBSDF(nullptr);
    blackMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *floorMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    floorMaterial->setName("Floor");
    floorMaterial->setReflectance(getColor(255, 255, 255));
    floorMaterial->setBSDF(new BSDF(&floorBSDF));
    floorMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *glassMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    glassMaterial->setName("Glass");
    glassMaterial->setReflectance(getColor(255, 255, 255));
    glassMaterial->setBSDF(new BSDF(&simpleGlassBSDF));
    glassMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *lensGlassMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    lensGlassMaterial->setName("LensGlass");
    lensGlassMaterial->setReflectance(getColor(255, 255, 255));
    lensGlassMaterial->setBSDF(new BSDF(&lensGlassBSDF));
    lensGlassMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *speakerGrillMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    speakerGrillMaterial->setName("SpeakerGrill");
    speakerGrillMaterial->setReflectance(getColor(255, 255, 255));
    speakerGrillMaterial->setBSDF(new BSDF(&speakerGrillBSDF));
    speakerGrillMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *screenMaskMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    screenMaskMaterial->setName("ScreenMask");
    screenMaskMaterial->setReflectance(getColor(5, 5, 5));
    screenMaskMaterial->setBSDF(new BSDF(&lambert));
    screenMaskMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *mattePlasticMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    mattePlasticMaterial->setName("MattePlastic");
    mattePlasticMaterial->setReflectance(getColor(0x52, 0x4f, 0x51));
    mattePlasticMaterial->setBSDF(new BSDF(&mattePlasticBSDF));
    mattePlasticMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *imageSensorMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    imageSensorMaterial->setName("ImageSensor");
    imageSensorMaterial->setReflectance(getColor(0x34, 0x2e, 0x38));
    imageSensorMaterial->setBSDF(new BSDF(&imageSensorBSDF));
    imageSensorMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *mirrorMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    mirrorMaterial->setName("Mirror");
    mirrorMaterial->setReflectance(getColor(255, 255, 255));
    mirrorMaterial->setBSDF(new BSDF(&mirrorBSDF));
    mirrorMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *flashColorMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    flashColorMaterial->setName("FlashColor");
    flashColorMaterial->setReflectance(getColor(0xE7, 0xE0, 0x7F));
    flashColorMaterial->setBSDF(new BSDF(&lambert));
    flashColorMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *backPlateMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    backPlateMaterial->setName("BackPlate");
    backPlateMaterial->setReflectance(getColor(255, 255, 255));
    backPlateMaterial->setBSDF(new BSDF(&backPlateBSDF));
    backPlateMaterial->setEmission(math::constants::Zero);

    SimpleBSDFMaterial *screenMaterial = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    screenMaterial->setName("Screen");
    screenMaterial->setEmissionNode(phoneScreenTexture.getMainOutput());
    screenMaterial->setEmission(math::loadVector(1.6f, 1.6f, 1.6f));
    screenMaterial->setReflectance(getColor(3, 3, 3));
    screenMaterial->setBSDF(new BSDF(&lambert));

    SimpleBSDFMaterial *strongLight = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    if (SCENE == UPRIGHT_SCENE || SCENE == BANNER_SCENE) strongLight->setEmission(math::loadVector(8.0f, 8.0f, 8.0f));
    else if (SCENE == FACE_ON_SCENE) strongLight->setEmission(math::loadVector(5.0f, 5.0f, 5.0f));
    strongLight->setReflectance(math::constants::Zero);
    strongLight->setName("StrongLight");

    SimpleBSDFMaterial *weakLight = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    weakLight->setEmission(math::loadVector(1.9f, 1.9f, 1.9f));
    weakLight->setReflectance(math::constants::Zero);
    weakLight->setName("WeakLight");

    SimpleBSDFMaterial *fillLight = rayTracer.getMaterialLibrary()->newMaterial<SimpleBSDFMaterial>();
    fillLight->setEmission(math::loadVector(11.4f, 11.4f, 11.4f));
    fillLight->setReflectance(math::constants::Zero);
    fillLight->setName("FillLight");

    // Create all scene geometry
    Mesh phone;
    phone.loadObjFileData(&phoneObj, 0);
    phone.bindMaterialLibrary(rayTracer.getMaterialLibrary(), defaultMaterial->getIndex());

    // Destroy file loaders
    phoneObj.destroy();

    KDTree kdtree;
    kdtree.configure(1000, math::constants::Zero);
    kdtree.analyzeWithProgress(&phone, 2);

    // Create scene objects
    SceneObject *phoneObject = scene.createSceneObject();
    if (USE_ACCELERATION_STRUCTURE) {
        phoneObject->setGeometry(&kdtree);
    }
    else {
        phoneObject->setGeometry(&phone);
    }
    phoneObject->setDefaultMaterial(defaultMaterial);
    phoneObject->setName("Phone");

    // Create the camera
    math::Vector cameraPos = math::loadVector(-39.408f, 12.216f, 45.728f);
    math::Vector target = math::loadVector(0.f, 0.f, 0.f);
    math::Vector up = math::loadVector(0.0f, 1.0f, 0.0f);
    math::real cameraPlaneSize = 0.25f;

    if (SCENE == FACE_ON_SCENE) {
        cameraPos = math::loadVector(0.f, 90.f, 0.f);
        up = math::loadVector(0.f, 0.f, -1.f);
    }
    else if (SCENE == UPRIGHT_SCENE) {
        target = math::loadVector(0.f, 8.8565f, -8.9672f);
        cameraPos = math::loadVector(0.f, 13.786f, -28.987f);
        cameraPlaneSize = 0.65f;
    }
    else if (SCENE == BANNER_SCENE) {
        target = math::loadVector(7.66829f, 7.63167f, -18.7725f);
        cameraPos = math::loadVector(8.2166f, 8.00816f, -31.4485f);
        cameraPlaneSize = 0.4992f;
    }

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
    if (SCENE == BANNER_SCENE) {
        if (HIGH_RESOLUTION) {
            camera->setResolutionX(1920 * 3);
            camera->setResolutionY(1080 * 3);
        }
        else {
            camera->setResolutionX(1920);
            camera->setResolutionY(1080);
        }
    }
    else {
        camera->setResolutionX(resolutionX);
        camera->setResolutionY(resolutionY);
    }
    group = camera;

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
        rayTracer.tracePixel(1044, 1063, &scene, group, &sceneBuffer);
    }
    else {
        rayTracer.traceAll(&scene, group, &sceneBuffer);
    }

    std::string fname = createUniqueRenderFilename("samsung_a8_demo", samplesPerPixel);
    std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".jpg";
    std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

    RawFile rawFile;
    rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);

    writeJpeg(imageFname.c_str(), &sceneBuffer, 95);

    sceneBuffer.destroy();
    speakerGrillTexture.destroy();
    phoneScreenTexture.destroy();
    backPlateTexture.destroy();
    groundRoughness.destroy();
    smudgeMap.destroy();
    rayTracer.destroy();
    phone.destroy();
    kdtree.destroy();

    std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
