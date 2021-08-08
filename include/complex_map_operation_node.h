#ifndef MANTARAY_COMPLEX_MAP_OPERATION_NODE_H
#define MANTARAY_COMPLEX_MAP_OPERATION_NODE_H

#include <piranha.h>

#include "complex_map_2d.h"
#include "complex_map_2d_node_output.h"
#include "object_reference_node.h"

namespace manta {

    enum class ComplexMapOperation {
        Fft,
        DftToCft,
        Roll,
        Normalize,
        Destroy
    };

    template <ComplexMapOperation T_Op>
    class ComplexMapOperationNode : public piranha::Node {
    public:
        static const int MaxAuxInputs = 32;

    public:
        ComplexMapOperationNode() {
            m_input = nullptr;
            m_target = nullptr;
            for (int i = 0; i < MaxAuxInputs; ++i) m_aux[i] = nullptr;

            m_auxInputs = 0;
        }

        virtual ~ComplexMapOperationNode() {
            /* void */
        }

    protected:
        virtual void _initialize() {
            /* void */
        }

        void addAuxInput(const std::string &name, int slot, bool modify) {
            registerInput(&m_aux[slot], name, modify);
        }

        math::real getAuxInputReal(int slot) {
            piranha::native_float in;
            m_aux[slot]->fullCompute(&in);

            return (math::real)in;
        }

        void doOp(ComplexMap2D *input, ComplexMap2D *target) {
            switch (T_Op) {
            case ComplexMapOperation::Fft:
                input->fft_multithreaded(target, 12);
                break;
            case ComplexMapOperation::DftToCft:
                input->cft(target, getAuxInputReal(0), getAuxInputReal(1));
                break;
            case ComplexMapOperation::Roll:
                input->roll(target);
                break;
            case ComplexMapOperation::Destroy:
                input->destroy();
                break;
            case ComplexMapOperation::Normalize:
                input->normalize(target);
                break;
            }
        }

        virtual void _evaluate() {
            ComplexMap2D *input = getObject<ComplexMap2D>(m_input);
            ComplexMap2D *target = getObject<ComplexMap2D>(m_target);

            doOp(input, target);
        }

        virtual void _destroy() {
            /* void */
        }

        virtual void registerInputs() {
            switch (T_Op) {
            case ComplexMapOperation::Fft:
            case ComplexMapOperation::DftToCft:
            case ComplexMapOperation::Roll:
            case ComplexMapOperation::Normalize:
                registerInput(&m_target, "target", true);
            }

            switch (T_Op) {
            case ComplexMapOperation::Fft:
            case ComplexMapOperation::DftToCft:
            case ComplexMapOperation::Roll:
            case ComplexMapOperation::Normalize:
                registerInput(&m_input, "in");
                break;
            case ComplexMapOperation::Destroy:
                registerInput(&m_input, "in", true);
            }

            switch (T_Op) {
            case ComplexMapOperation::DftToCft:
                addAuxInput("width", 0, false);
                addAuxInput("height", 1, false);
                break;
            }
        }

        virtual void registerOutputs() {
            /* void */
        }

    protected:
        piranha::pNodeInput m_target;
        piranha::pNodeInput m_input;
        piranha::pNodeInput m_aux[32];

        int m_auxInputs;
    };

} /* namespace manta */

#endif /* MANTARAY_COMPLEX_MAP_OPERATION_NODE_H */
