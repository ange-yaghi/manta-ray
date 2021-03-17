#ifndef MANTARAY_UV_OPERATION_OUTPUT_H
#define MANTARAY_UV_OPERATION_OUTPUT_H

#include "vector_node_output.h"

#include "surface_interaction_node.h"

namespace manta {

    enum class UvOperation {
        Wrap
    };

    template <UvOperation op>
    class UvOperationOutput : public VectorNodeOutput {
    public:

    public:
        UvOperationOutput() {
            m_input = nullptr;
        }

        virtual ~UvOperationOutput() {
            /* void */
        }

        static inline math::Vector doOp(const math::Vector &uv) {
            switch (op) {
            case UvOperation::Wrap:
                return wrap(uv);
            default:
                return math::constants::Zero;
            }
        }

        static inline math::Vector wrap(const math::Vector &uv) {
            const math::real u = math::getX(uv);
            const math::real v = math::getY(uv);

            if (u <= (math::real)1.0 && u >= (math::real)0.0 &&
                v <= (math::real)1.0 && v >= (math::real)0.0) 
            {
                return uv;
            }

            math::real fu = fmodf(u, (math::real)1.0);
            math::real fv = fmodf(v, (math::real)1.0);

            if (fu < (math::real)0.0) fu += (math::real)1.0;
            if (fv < (math::real)0.0) fv += (math::real)1.0;

            return math::loadVector(fu, fv);
        }

        virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const {
            math::Vector input;
            static_cast<VectorNodeOutput *>(m_input)->sample(surfaceInteraction, &input);
            *reinterpret_cast<math::Vector *>(target) = doOp(input);
        }

        virtual void discreteSample2d(int x, int y, void *target) const {
            math::Vector input;
            static_cast<VectorNodeOutput *>(m_input)->discreteSample2d(x, y, &input);
            *reinterpret_cast<math::Vector *>(target) = doOp(input);
        }

        virtual void getDataReference(const void **target) const {
            *target = nullptr;
        }

        virtual void _evaluateDimensions() {
            VectorNodeOutput *input = static_cast<VectorNodeOutput *>(m_input);
            input->evaluateDimensions();

            const int dimensions = input->getDimensions();
            setDimensions(dimensions);

            for (int i = 0; i < dimensions; i++) {
                setDimensionSize(i, input->getSize(i));
            }
        }

        piranha::pNodeInput *getConnection() { return &m_input; }

    protected:
        virtual void registerInputs() {
            registerInput(&m_input);
        }

    protected:
        piranha::pNodeInput m_input;
    };

} /* namespace manta */

#endif /* MANTARAY_UV_OPERATION_OUTPUT_H */
