#ifndef MANTARAY_UV_OPERATION_OUTPUT_H
#define MANTARAY_UV_OPERATION_OUTPUT_H

#include "vector_node_output.h"

#include "surface_interaction_node.h"

namespace manta {

    enum UV_OPERATION {
        WRAP
    };

    template <UV_OPERATION op>
    class UvOperationOutput : public VectorNodeOutput {
    public:

    public:
        UvOperationOutput() {
            m_input = nullptr;
        }

        virtual ~UvOperationOutput() {
            /* void */
        }

        static inline bool doOp(const IntersectionPoint *surfaceInteraction, IntersectionPoint *output) {
            switch (op) {
            case WRAP:
                return wrap(surfaceInteraction, output);
            default:
                return false;
            }
        }

        static inline bool wrap(const IntersectionPoint *surfaceInteraction, IntersectionPoint *output) {
            math::real u = math::getX(surfaceInteraction->m_textureCoodinates);
            math::real v = math::getY(surfaceInteraction->m_textureCoodinates);

            if (u <= (math::real)1.0 && u >= (math::real)0.0 &&
                v <= (math::real)1.0 && v >= (math::real)0.0) {
                return false;
            }

            math::real fu = fmodf(u, (math::real)1.0);
            math::real fv = fmodf(v, (math::real)1.0);

            if (fu < (math::real)0.0) fu += (math::real)1.0;
            if (fv < (math::real)0.0) fv += (math::real)1.0;

            math::Vector newCoordinates = math::loadVector(fu, fv);

            *output = *surfaceInteraction;
            output->m_textureCoodinates = newCoordinates;

            return true;
        }

        virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const {
            IntersectionPoint newIntersectionPoint;
            bool changed = doOp(surfaceInteraction, &newIntersectionPoint);

            if (!changed) static_cast<VectorNodeOutput *>(m_input)->sample(surfaceInteraction, target);
            else static_cast<VectorNodeOutput *>(m_input)->sample(&newIntersectionPoint, target);
        }

        virtual void discreteSample2d(int x, int y, void *target) const {
            static_cast<VectorNodeOutput *>(m_input)->discreteSample2d(x, y, target);
        }

        virtual void getDataReference(const void **target) const {
            *target = nullptr;
        }

        virtual void _evaluateDimensions() {
            VectorNodeOutput *input = static_cast<VectorNodeOutput *>(m_input);
            input->evaluateDimensions();

            int dimensions = input->getDimensions();
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

#endif /* MANTARAY_UNARY_NODE_OUTPUT_H */
