#ifndef MANTARAY_FRAUNHOFER_DIFFRACTION_NODE_H
#define MANTARAY_FRAUNHOFER_DIFFRACTION_NODE_H

#include "node.h"

#include "vector_map_2d_node_output.h"
#include "image_plane.h"
#include "cmf_table.h"
#include "spectrum.h"

namespace manta {

    class ComplexMap2D;

    class FraunhoferDiffractionNode : public Node {
    public:
        FraunhoferDiffractionNode();
        ~FraunhoferDiffractionNode();

        void generate();

    protected:
        VectorMap2D m_result;
        ImagePlane m_intermediate;
        ImagePlane *m_imagePlane;
        ComplexMap2D *m_diffractionCft;
        CmfTable m_colorTable;
        Spectrum *m_spectrum;
        Spectrum m_defaultSpectrum;

        int m_size;
        math::real m_cftSize;
        math::real m_sensorSize;
        math::real m_sensorDistance;

        int m_startWavelength;
        int m_endWavelength;
        int m_wavelengthStep;
        bool m_normalizeColor;

    protected:
        void normalize();

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual void registerInputs();
        virtual void registerOutputs();

        VectorMap2DNodeOutput m_output;

        piranha::pNodeInput m_spectrumPathInput;
        piranha::pNodeInput m_diffractionCftInput;
        piranha::pNodeInput m_diffractionCftSizeInput;
        piranha::pNodeInput m_sensorSizeInput;
        piranha::pNodeInput m_sensorDistanceInput;
        piranha::pNodeInput m_imagePlaneInput;
        piranha::pNodeInput m_sizeInput;
        piranha::pNodeInput m_cmfTablePathInput;
        piranha::pNodeInput m_normalizeColorInput;
        piranha::pNodeInput m_startWavelengthInput;
        piranha::pNodeInput m_endWavelengthInput;
        piranha::pNodeInput m_wavelengthStepInput;
    };

} /* namespace manta */

#endif /* MANTARAY_FRAUNHOFER_DIFFRACTION_NODE_H */
