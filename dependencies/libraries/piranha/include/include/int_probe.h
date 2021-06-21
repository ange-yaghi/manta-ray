#ifndef PIRANHA_INT_PROBE_NODE_H
#define PIRANHA_INT_PROBE_NODE_H

#include "node.h"

#include "fundamental_types.h"

#include <string>
#include <iostream>

namespace piranha {

    class IntProbeNode : public Node {
    public:
        IntProbeNode() {
            m_input = nullptr;
        }

        ~IntProbeNode() {
            /* void */
        }

    protected:
        virtual void _evaluate() {
            piranha::native_int data;
            m_input->fullCompute(&data);
        }

        virtual void registerInputs() {
            registerInput(&m_input, "__in");
        }

        virtual void registerOutputs() {
            /* void */
        }

    protected:
        pNodeInput m_input;
    };

} /* namespace piranha */

#endif /* PIRANHA_INT_PROBE_NODE_H */
