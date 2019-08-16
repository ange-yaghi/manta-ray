#ifndef MANTARAY_UNARY_NODE_OUTPUT_H
#define MANTARAY_UNARY_NODE_OUTPUT_H

#include "vector_node_output.h"

namespace manta {

    enum UNARY_OPERATION {
        NORMALIZE,
        NEGATE,
        MAGNITUDE,
        MAX_COMPONENT,
        ABSOLUTE
    };

    template <UNARY_OPERATION op>
    class UnaryNodeOutput : public VectorNodeOutput {
    public:

    public:
        UnaryNodeOutput() {
            m_input = nullptr;
        }

        virtual ~UnaryNodeOutput() {
            /* void */
        }

        static inline math::Vector doOp(const math::Vector &input) {
            switch (op) {
            case NORMALIZE:
                return math::normalize(input);
            case MAGNITUDE:
                return math::magnitude(input);
            case NEGATE:
                return math::negate(input);
            case MAX_COMPONENT:
                return math::maxComponent(input);
            case ABSOLUTE:
                return math::abs(input);
            default:
                return math::constants::Zero;
            }
        }

        virtual void sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
            math::Vector *target = reinterpret_cast<math::Vector *>(_target);

            math::Vector input;
            static_cast<VectorNodeOutput *>(m_input)->sample(surfaceInteraction, (void *)&input);

            *target = doOp(input);
        }

        virtual void discreteSample2d(int x, int y, void *_target) const {
            math::Vector *target = reinterpret_cast<math::Vector *>(_target);

            math::Vector input;
            static_cast<VectorNodeOutput *>(m_input)
                ->discreteSample2d(x, y, (void *)&input);

            *target = doOp(input);
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
