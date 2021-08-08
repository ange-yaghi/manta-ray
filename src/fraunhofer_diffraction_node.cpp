#include "../include/fraunhofer_diffraction_node.h"

#include "../include/complex_map_2d.h"
#include "../include/cft_estimator_2d.h"
#include "../include/spectrum.h"
#include "../include/cmf_table.h"
#include "../include/session.h"
#include "../include/console.h"
#include "../include/color.h"

manta::FraunhoferDiffractionNode::FraunhoferDiffractionNode() {
    m_imagePlaneInput = nullptr;
    m_imagePlane = nullptr;
    m_diffractionCft = nullptr;
    m_diffractionCftInput = nullptr;
    m_diffractionCftSizeInput = nullptr;
    m_sizeInput = nullptr;
    m_cmfTablePathInput = nullptr;
    m_sensorSizeInput = nullptr;
    m_sensorDistanceInput = nullptr;
    m_spectrumPathInput = nullptr;
    m_normalizeColorInput = nullptr;
    m_startWavelengthInput = nullptr;
    m_endWavelengthInput = nullptr;
    m_wavelengthStepInput = nullptr;

    m_size = 0;
    m_cftSize = (math::real)0.0;
    m_sensorSize = (math::real)0.0;
    m_sensorDistance = (math::real)0.0;
    m_spectrum = nullptr;
    m_startWavelength = 380;
    m_endWavelength = 750;
    m_wavelengthStep = 2;
    m_normalizeColor = true;
}

manta::FraunhoferDiffractionNode::~FraunhoferDiffractionNode() {
    /* void */
}

void manta::FraunhoferDiffractionNode::generate() {
    static constexpr int SampleBufferCapacity = 128;
    static constexpr math::real NanometreToMillimetre = (math::real)1E-6;

    m_intermediate.initialize(m_size, m_size);
    m_intermediate.setFilter(m_imagePlane->getFilter());

    const int diffractionPatternSize = m_diffractionCft->getWidth();

    CftEstimator2D estimator;
    estimator.initialize(m_diffractionCft, m_cftSize, m_cftSize);

    const math::real_d sensorSize = m_sensorSize;
    const math::real_d sensorElementSize = sensorSize / m_size;

    const int startWavelength = m_startWavelength;
    const int endWavelength = m_endWavelength;
    const int wavelengthStep = m_wavelengthStep;
    const int wavelengthSteps = (endWavelength - startWavelength) / wavelengthStep + 1;

    StackAllocator s;
    s.initialize(10 * MB);

    ImageSample *samples = new ImageSample[SampleBufferCapacity];
    int sampleCount = 0;

    Spectrum spectrum;
    spectrum.initialize(wavelengthSteps, (math::real)startWavelength, (math::real)endWavelength, nullptr);

    for (int wavelength = startWavelength; wavelength <= endWavelength; wavelength += wavelengthStep) {
        if (getProgram()->isKilled()) break;

        spectrum.clear();
        const int wave_i = (int)(((math::real_d)wavelength - startWavelength) / (math::real_d)wavelengthStep + 0.5);
        const math::real prev = spectrum.getValueDiscrete(wave_i);
        spectrum.set(wave_i, 1.0f);

        ColorXyz xyzColor = m_colorTable.spectralToXyz(&spectrum);
        const math::Vector color =
            math::loadVector(
                (math::real)xyzColor.x,
                (math::real)xyzColor.y,
                (math::real)xyzColor.z,
                (math::real)1.0);
        const math::Vector rgbColor = m_colorTable.xyzToRgb(xyzColor);

        std::stringstream ss;
        ss << "wavelength=" << wavelength << "nm [######]\n";
        Session::get().getConsole()->out(ss.str(), rgbColor);

        for (int j = 0; j < diffractionPatternSize; ++j) {
            for (int i = 0; i < diffractionPatternSize; ++i) {
                math::Complex value = estimator.sampleDiscrete(i, j);
                value = value * value.conjugate();

                // freq_x = x / (wavelength * distance)

                const math::real_d freq_x = estimator.getFrequencyX(i);
                const math::real_d freq_y = estimator.getFrequencyY(j);

                const math::real_d x = ((freq_x * wavelength * NanometreToMillimetre * m_sensorDistance) + sensorSize / 2) / sensorElementSize;
                const math::real_d y = ((freq_y * wavelength * NanometreToMillimetre * m_sensorDistance) + sensorSize / 2) / sensorElementSize;

                samples[sampleCount++] = {
                    math::loadScalar(value.r * 100 * m_spectrum->getValueContinuous((math::real)wavelength)),
                    { (math::real)x, (math::real)y }
                };

                if (sampleCount >= SampleBufferCapacity) {
                    m_intermediate.processSamples(samples, sampleCount, &s);
                    sampleCount = 0;
                }
            }
        }

        if (sampleCount >= SampleBufferCapacity) {
            m_intermediate.processSamples(samples, sampleCount, &s);
            sampleCount = 0;
        }

        m_intermediate.normalize();

        const math::real_d energySpread = 1 / (wavelength / (math::real_d)startWavelength);
        
        for (int i = 0; i < m_size; ++i) {
            for (int j = 0; j < m_size; ++j) { 
                m_imagePlane->add(math::mul(rgbColor, math::mul(m_intermediate.sample(i, j), math::loadScalar(energySpread))), i, j);
            }
        }
    }

    spectrum.destroy();

    normalize();
}

void manta::FraunhoferDiffractionNode::normalize() {
    math::real_d totalFlux = 0;
    math::Vector totalColorFlux = math::constants::Zero;
    for (int i = 0; i < m_size; i++) {
        for (int j = 0; j < m_size; j++) {
            const math::real_d luminance =
                math::getScalar(math::dot(math::loadVector(0.2126f, 0.7152f, 0.0722f), m_imagePlane->sample(i, j)));
            totalFlux += luminance;
            totalColorFlux = math::add(totalColorFlux, m_imagePlane->sample(i, j));
        }
    }

    // Normalize
    m_result.initialize(m_size, m_size);
    const math::real ds = 1 / (math::real)m_size;
    const math::Vector totalEnergy = math::loadScalar(1 / (ds * ds));
    const math::Vector norm = (m_normalizeColor)
        ? math::div(totalEnergy, totalColorFlux)
        : math::div(totalEnergy, math::loadScalar(totalFlux));
    for (int j = 0; j < m_size; ++j) {
        for (int i = 0; i < m_size; ++i) {
            math::Vector rgbColor = m_imagePlane->sample(i, j);
            rgbColor = math::mul(rgbColor, norm);
            rgbColor = math::mask(rgbColor, math::constants::MaskOffW);

            m_result.set(rgbColor, i, j);
        }
    }
}

void manta::FraunhoferDiffractionNode::_initialize() {
    /* void */
}

void manta::FraunhoferDiffractionNode::_evaluate() {
    m_diffractionCft = getObject<ComplexMap2D>(m_diffractionCftInput);
    m_imagePlane = getObject<ImagePlane>(m_imagePlaneInput);

    piranha::native_int size, startWavelength, endWavelength, wavelengthStep;
    m_sizeInput->fullCompute(&size);
    m_startWavelengthInput->fullCompute(&startWavelength);
    m_endWavelengthInput->fullCompute(&endWavelength);
    m_wavelengthStepInput->fullCompute(&wavelengthStep);

    piranha::native_bool normalizeColor;
    m_normalizeColorInput->fullCompute(&normalizeColor);

    piranha::native_string colorTablePath, spectrumPath;
    m_cmfTablePathInput->fullCompute(&colorTablePath);
    m_spectrumPathInput->fullCompute(&spectrumPath);

    piranha::native_float cftSize, sensorSize, sensorDistance;
    m_diffractionCftSizeInput->fullCompute(&cftSize);
    m_sensorSizeInput->fullCompute(&sensorSize);
    m_sensorDistanceInput->fullCompute(&sensorDistance);

    m_colorTable.loadCsv(colorTablePath.c_str());
    m_defaultSpectrum.loadCsv(spectrumPath.c_str());

    m_spectrum = &m_defaultSpectrum;
    m_size = size;
    m_cftSize = (math::real)cftSize;
    m_sensorSize = (math::real)sensorSize;
    m_sensorDistance = (math::real)sensorDistance;
    m_startWavelength = startWavelength;
    m_endWavelength = endWavelength;
    m_wavelengthStep = wavelengthStep;
    m_normalizeColor = normalizeColor;

    m_output.setMap(&m_result);

    generate();
}

void manta::FraunhoferDiffractionNode::_destroy() {
    m_colorTable.destroy();
    m_intermediate.destroy();
    m_defaultSpectrum.destroy();
    m_result.destroy();
}

void manta::FraunhoferDiffractionNode::registerInputs() {
    registerInput(&m_diffractionCftInput, "pattern_cft");
    registerInput(&m_imagePlaneInput, "image_plane");
    registerInput(&m_sizeInput, "resolution");
    registerInput(&m_sensorSizeInput, "sensor_size");
    registerInput(&m_sensorDistanceInput, "sensor_distance");
    registerInput(&m_diffractionCftSizeInput, "cft_size");
    registerInput(&m_cmfTablePathInput, "cmf_table_path");
    registerInput(&m_spectrumPathInput, "spectrum_path");
    registerInput(&m_wavelengthStepInput, "wavelength_step");
    registerInput(&m_startWavelengthInput, "start_wavelength");
    registerInput(&m_endWavelengthInput, "end_wavelength");
    registerInput(&m_normalizeColorInput, "normalize_color");
}

void manta::FraunhoferDiffractionNode::registerOutputs() {
    setPrimaryOutput("__out");
    registerOutput(&m_output, "__out");
}
