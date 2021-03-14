#include "../include/fraunhofer_diffraction.h"

#include "../include/image_byte_buffer.h"
#include "../include/aperture.h"
#include "../include/complex_map_2d.h"
#include "../include/cft_estimator_2d.h"
#include "../include/spectrum.h"
#include "../include/color.h"
#include "../include/cmf_table.h"
#include "../include/mipmap.h"

// Temp
#include "../include/texture_node.h"
#include "../include/intersection_point.h"

#include <thread>

manta::FraunhoferDiffraction::FraunhoferDiffraction() {
    m_colorTable = nullptr;
    m_sourceSpectrum = nullptr;

    m_physicalSensorWidth = (math::real)0.0;
    m_sensorElementWidth = (math::real)0.0;
    
    m_apertureSamplesInput = nullptr;
    m_textureSamplesInput = nullptr;
    m_minWavelengthInput = nullptr;
    m_maxWavelengthInput = nullptr;
    m_wavelengthStepInput = nullptr;
    m_safetyFactorInput = nullptr;
    m_frequencyMultiplierInput = nullptr;
    m_dirtMapInput = nullptr;
    m_apertureInput = nullptr;
    m_resolutionInput = nullptr;
    m_sensorWidthInput = nullptr;
}

manta::FraunhoferDiffraction::~FraunhoferDiffraction() {
    /* void */
}

void manta::FraunhoferDiffraction::generate(const Aperture *aperture, 
        const VectorMap2D *dirtMap, int outputResolution, math::real physicalSensorWidth, 
        CmfTable *colorTable, Spectrum *sourceSpectrum, const Settings *settingsIn) 
{
    Settings settings;
    if (settingsIn == nullptr) setDefaultSettings(&settings);
    else settings = *settingsIn;
    
    int minWavelength = settings.minWavelength;
    int maxWavelength = settings.maxWavelength;
    int wavelengthStep = settings.wavelengthStep;

    math::real apertureRadius = aperture->getRadius();
    math::real apertureRadius_inv = 1 / (2 * apertureRadius);
    math::real sensorWidth = physicalSensorWidth;
    math::real sensorElementWidth = physicalSensorWidth / outputResolution;

    m_physicalSensorWidth = physicalSensorWidth;
    m_sensorElementWidth = sensorElementWidth;
    m_colorTable = colorTable;
    m_sourceSpectrum = sourceSpectrum;

    math::real_d minFrequencyStep = sensorElementWidth / (maxWavelength * 1e-6);
    math::real_d maxFrequencyStep = sensorElementWidth / (minWavelength * 1e-6);

    int estimatorSamples = 0;
    math::real_d sampleWindow = 0.0;
    math::real_d maxFreq = (sensorWidth / 2) / (minWavelength * 1e-6);

    sampleWindow = CftEstimator2D::getMinPhysicalDim(minFrequencyStep / settings.frequencyMultiplier, apertureRadius * 2 * settings.safetyFactor);
    estimatorSamples = settings.maxSamples;

    math::real dx = (math::real)(sampleWindow / estimatorSamples);
    math::real dy = (math::real)(sampleWindow / estimatorSamples);
    math::real cx = (math::real)(sampleWindow / 2);
    math::real cy = (math::real)(sampleWindow / 2);

    // Generate aperture function
    ComplexMap2D apertureFunction;
    apertureFunction.initialize(estimatorSamples, estimatorSamples);
    if (settings.saveApertureFunction) {
        m_apertureFunction.initialize(estimatorSamples, estimatorSamples);
    }

    IntersectionPoint a;
    for (int i = 0; i < estimatorSamples; i++) {
        math::real x = i * dx - cx;
        math::real u = (x * apertureRadius_inv) + (math::real)0.5;

        for (int j = 0; j < estimatorSamples; j++) {
            math::real y = j * dy - cy;
            math::real v = (y * apertureRadius_inv) + (math::real)0.5;

            a.m_textureCoodinates = math::loadVector(u, v);

            math::real dirt = (math::real)1.0;
            if (dirtMap != nullptr) {
                math::Vector dirtV;
                dirtV = dirtMap->triangleSample(u, v);
                dirt = math::getScalar(dirtV);
            }
            else if (m_dirtMapInput != nullptr) {
                math::Vector dirtV;
                static_cast<VectorNodeOutput *>(m_dirtMapInput)->sample(&a, (void *)&dirtV);
                dirt = math::getScalar(dirtV);
            }

            // Super-sampling
            int samplesInFilter = 0;
            int totalSamples = 0;
            for (int si = -2; si <= 2; si++) {
                for (int sj = -2; sj <= 2; sj++) {
                    totalSamples++;
                    if (aperture->filter(x + si * (dx / 4), y + sj * (dy / 4))) {
                        samplesInFilter++;
                    }
                }
            }

            math::real_d apF = (math::real_d)samplesInFilter / totalSamples;
            apertureFunction.set(math::Complex(apF * dirt, (math::real_d)0.0), i, j);

            if (settings.saveApertureFunction) {
                m_apertureFunction.set(math::loadScalar((math::real)(apF * dirt)), i, j);
            }
        }
    }

    CftEstimator2D estimator;
    estimator.initialize(&apertureFunction, sampleWindow, sampleWindow);
    apertureFunction.destroy();

    m_diffractionPattern.initialize(outputResolution, outputResolution);
    generateMap(&estimator, &settings, 12, &m_diffractionPattern);
    estimator.destroy();

    normalize();
}

void manta::FraunhoferDiffraction::destroy() {
    m_diffractionPattern.destroy();
    m_apertureFunction.destroy();
}

manta::math::Vector manta::FraunhoferDiffraction::samplePattern(math::real dx, math::real dy) const {
    math::real u, v;

    u = dx / (m_physicalSensorWidth) + (math::real)0.5;
    v = dy / (m_physicalSensorWidth) + (math::real)0.5;

    return m_diffractionPattern.sample(u, v);
}

void manta::FraunhoferDiffraction::normalize() {
    int res = m_diffractionPattern.getWidth();

    math::real maxY = (math::real)0.0;
    for (int i = 0; i < res; i++) {
        for (int j = 0; j < res; j++) {
            math::Vector xyzColor = m_diffractionPattern.get(i, j);
            math::real Y = math::getY(xyzColor);
            if (Y > maxY) {
                maxY = Y;
            }
        }
    }

    if (maxY == (math::real)0.0) return;

    // Convert to RGB
    for (int i = 0; i < res; i++) {
        for (int j = 0; j < res; j++) {
            math::Vector xyzColor = m_diffractionPattern.get(i, j);
            xyzColor = math::mul(math::loadScalar(1 / maxY), xyzColor);

            ColorXyz xyzColorT = ColorXyz(math::getX(xyzColor), math::getY(xyzColor), math::getZ(xyzColor));
            math::Vector rgbColor = m_colorTable->xyzToRgb(xyzColorT);

            m_diffractionPattern.set(rgbColor, i, j);
        }
    }

    math::real ds = 1 / (math::real)res;
    math::Vector totalFlux = math::constants::Zero;
    for (int i = 0; i < res; i++) {
        for (int j = 0; j < res; j++) {
            totalFlux = math::add(totalFlux, m_diffractionPattern.get(i, j));
        }
    }
    math::Vector totalEnergy = math::loadScalar(1 / (ds * ds));

    // Normalize
    math::Vector norm = math::div(totalEnergy, totalFlux);
    for (int i = 0; i < res; i++) {
        for (int j = 0; j < res; j++) {
            math::Vector rgbColor = m_diffractionPattern.get(i, j);
            rgbColor = math::mul(rgbColor, norm);
            rgbColor = math::mask(rgbColor, math::constants::MaskOffW);

            m_diffractionPattern.set(rgbColor, i, j);
        }
    }
}

void manta::FraunhoferDiffraction::setDefaultSettings(Settings *settings) {
    settings->maxSamples = 4096;
    settings->textureSamples = 10;
    settings->maxWavelength = 780;
    settings->minWavelength = 380;
    settings->wavelengthStep = 5;
    settings->frequencyMultiplier = 3.0;
    settings->safetyFactor = (math::real_d)1.1;

    settings->saveApertureFunction = false;
}

void manta::FraunhoferDiffraction::generateMap(const CftEstimator2D *estimator, const Settings *settings, int threadCount, VectorMap2D *target) const {
    int size = target->getHeight();

    int div = size / threadCount;
    if (div == 0) div = size;

    std::thread **threads = StandardAllocator::Global()->allocate<std::thread *>(threadCount);

    // Distribute to threads
    for (int i = 0; i < threadCount; i++) {
        int start = i * div;
        int end = (i + 1) * div;

        if (end >= size || i == (threadCount - 1)) {
            end = size;
        }

        if (start < end) {
            threads[i] = new std::thread(&FraunhoferDiffraction::_generateMap, this, estimator, settings, start, end, target);
        }
        else {
            threads[i] = nullptr;
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < threadCount; i++) {
        if (threads[i] != nullptr) {
            threads[i]->join();
            delete threads[i];
        }
    }

    // Free temporary memory
    StandardAllocator::Global()->free(threads, threadCount);
}

void manta::FraunhoferDiffraction::_generateMap(const CftEstimator2D *estimator, 
        const Settings *settings, int startRow, int endRow, VectorMap2D *target) const {
    int res = target->getWidth();

    math::real_d sdx = m_sensorElementWidth;
    math::real_d sdy = m_sensorElementWidth;
    math::real_d scx = m_physicalSensorWidth / 2;
    math::real_d scy = m_physicalSensorWidth / 2;
    math::real_d maxFreqX = estimator->getHorizontalFreqRange();
    math::real_d maxFreqY = estimator->getVerticalFreqRange();

    int startWavelength = settings->minWavelength;
    int endWavelength = settings->maxWavelength;
    int wavelengthStep = settings->wavelengthStep;
    int textureSamples = settings->textureSamples;

    int steps = (endWavelength - startWavelength) / settings->wavelengthStep + 1;
    Spectrum spectrum;
    spectrum.initialize(steps, (math::real)startWavelength, (math::real)endWavelength, nullptr);

    for (int i = startRow; i < endRow; i++) {
        math::real_d x = i * sdx - scx;

        for (int j = 0; j < res; j++) {
            math::real_d y = j * sdy - scy;
            spectrum.clear();

            for (int wavelength = startWavelength; 
                    wavelength <= endWavelength; wavelength += wavelengthStep) {
                math::real_d freqSpace = (sdx * 1E6 / wavelength) * 1.0;

                for (int s = 0; s < textureSamples; s++) {
                    math::real randX = math::uniformRandom() - (math::real)0.5;
                    math::real randY = math::uniformRandom() - (math::real)0.5;

                    math::real_d freq_x = ((x * 1E6) / wavelength);
                    math::real_d freq_y = ((y * 1E6) / wavelength);

                    freq_x += randX * freqSpace;
                    freq_y += randY * freqSpace;

                    if (abs(freq_x) > maxFreqX) continue;
                    if (abs(freq_y) > maxFreqY) continue;

                    math::Complex v = estimator->sample(freq_x, freq_y, (sdx / wavelength) * 1E6);
                    v = v * v.conjugate();

                    math::real src = (m_sourceSpectrum != nullptr)
                        ? m_sourceSpectrum->getValueContinuous((math::real)wavelength)
                        : (math::real)1.0;
                    int wave_i = (int)((wavelength - startWavelength) / (math::real_d)wavelengthStep + 0.5);
                    math::real prev = spectrum.getValueDiscrete(wave_i);
                    spectrum.set(wave_i, prev + src * (math::real)v.r);
                }
            }

            ColorXyz xyzColor = m_colorTable->spectralToXyz(&spectrum);
            target->set(
                math::loadVector(
                    (math::real)xyzColor.x,
                    (math::real)xyzColor.y,
                    (math::real)xyzColor.z), i, j);
        }
    }

    spectrum.destroy();
}

void manta::FraunhoferDiffraction::_initialize() {
    m_diffractionPatternOutput.initialize();
    m_aperturePatternOutput.initialize();
}

void manta::FraunhoferDiffraction::_evaluate() {
    // Populate settings
    Settings settings;
    m_apertureSamplesInput->fullCompute((void *)&settings.maxSamples);
    m_textureSamplesInput->fullCompute((void *)&settings.textureSamples);
    m_minWavelengthInput->fullCompute((void *)&settings.minWavelength);
    m_maxWavelengthInput->fullCompute((void *)&settings.maxWavelength);
    m_wavelengthStepInput->fullCompute((void *)&settings.wavelengthStep);
    m_frequencyMultiplierInput->fullCompute((void *)&settings.frequencyMultiplier);
    m_safetyFactorInput->fullCompute((void *)&settings.safetyFactor);
    settings.saveApertureFunction = true;

    int resolution;
    m_resolutionInput->fullCompute((void *)&resolution);
    Aperture *aperture = getObject<Aperture>(m_apertureInput);

    piranha::native_float sensorWidth;
    m_sensorWidthInput->fullCompute((void *)&sensorWidth);

    piranha::native_string cmfTablePath;
    m_cmfTableInput->fullCompute((void *)&cmfTablePath);

    CmfTable table;
    table.loadCsv(cmfTablePath.c_str());
    generate(aperture, nullptr, resolution, (math::real)sensorWidth, &table, nullptr, &settings);
    table.destroy();

    m_aperturePatternOutput.setMap(&m_apertureFunction);
    m_diffractionPatternOutput.setMap(&m_diffractionPattern);
}

void manta::FraunhoferDiffraction::_destroy() {
    /* void */
}

void manta::FraunhoferDiffraction::registerInputs() {
    registerInput(&m_apertureSamplesInput, "aperture_samples");
    registerInput(&m_textureSamplesInput, "texture_samples");
    registerInput(&m_minWavelengthInput, "min_wavelength");
    registerInput(&m_maxWavelengthInput, "max_wavelength");
    registerInput(&m_wavelengthStepInput, "wavelength_step");
    registerInput(&m_safetyFactorInput, "safety_factor");
    registerInput(&m_frequencyMultiplierInput, "frequency_multiplier");
    registerInput(&m_dirtMapInput, "dirt_map");
    registerInput(&m_apertureInput, "aperture");
    registerInput(&m_sensorWidthInput, "sensor_width");
    registerInput(&m_resolutionInput, "resolution");
    registerInput(&m_cmfTableInput, "cmf_table_path");
}

void manta::FraunhoferDiffraction::registerOutputs() {
    registerOutput(&m_diffractionPatternOutput, "diffraction");
    registerOutput(&m_aperturePatternOutput, "aperture_function");
}
