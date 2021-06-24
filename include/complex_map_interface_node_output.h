#ifndef MANTARAY_COMPLEX_MAP_INTERFACE_NODE_OUTPUT_H
#define MANTARAY_COMPLEX_MAP_INTERFACE_NODE_OUTPUT_H

#include "vector_node_output.h"

#include "complex_map_2d_node.h"
#include "object_reference_node_output.h"
#include "complex_node_output.h"

namespace manta {

    enum class ComplexMapProperty {
        Width,
        Height
    };

    template <ComplexMapProperty T_op>
    class ComplexMapInterfaceNodeOutput : public piranha::NodeOutput {
    public:
        ComplexMapInterfaceNodeOutput() : piranha::NodeOutput(getType()) {
            /* void */
        }

        virtual ~ComplexMapInterfaceNodeOutput() {
            /* void */
        }

        static inline const piranha::ChannelType *getType() {
            switch (T_op) {
            case ComplexMapProperty::Width:
            case ComplexMapProperty::Height:
                return &ComplexNodeOutput::ComplexType;
            default:
                return nullptr;
            }
        }

        static inline void doOp(ComplexMap2D *map, void *target_) {
            switch (T_op) {
            case ComplexMapProperty::Width:
                *reinterpret_cast<piranha::native_int *>(target_) = (piranha::native_int)map->getWidth();
                break;
            case ComplexMapProperty::Height:
                *reinterpret_cast<piranha::native_int *>(target_) = (piranha::native_int)map->getHeight();
                break;
            }
        }

        virtual void fullCompute(void *target) const {
            ComplexMap2D *map = getObject<ComplexMap2D>(m_input);
            doOp(map, target);
        }

        piranha::pNodeInput *getInputConnection() { return &m_input; }

    protected:
        piranha::pNodeInput m_input;
    };

} /* namespace manta */

#endif /* MANTARAY_COMPLEX_MAP_INTERFACE_NODE_OUTPUT_H */
