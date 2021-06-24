#ifndef MANTARAY_COMPLEX_MAP_INTERFACE_NODE_H
#define MANTARAY_COMPLEX_MAP_INTERFACE_NODE_H

#include <piranha.h>

#include "complex_map_interface_node_output.h"

namespace manta {

    class ComplexMapInterfaceNode : public piranha::Node {
    public:
        ComplexMapInterfaceNode() {
            m_input = nullptr;
        }

        virtual ~ComplexMapInterfaceNode() {
            /* void */
        }

    protected:
        virtual void _initialize() {
            /* void */
        }

        virtual void _evaluate() {
            /* void */
        }

        virtual void _destroy() {
            /* void */
        }

        virtual void registerOutputs() {
            registerOutput(&m_width, "width");
            registerOutput(&m_height, "height");
        }

        virtual void registerInputs() {
            setInterfaceInput(&m_input);

            registerInput(&m_input, "__in");
            registerInput(m_width.getInputConnection(), "__in");
            registerInput(m_height.getInputConnection(), "__in");
        }

    protected:
        piranha::pNodeInput m_input;

        ComplexMapInterfaceNodeOutput<ComplexMapProperty::Width> m_width;
        ComplexMapInterfaceNodeOutput<ComplexMapProperty::Height> m_height;
    };

} /* namespace manta */

#endif /* MANTARAY_COMPLEX_MAP_INTERFACE_NODE_H */
