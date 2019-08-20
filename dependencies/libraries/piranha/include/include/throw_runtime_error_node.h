#ifndef PIRANHA_THROW_RUNTIME_ERROR_NODE_H
#define PIRANHA_THROW_RUNTIME_ERROR_NODE_H

#include "node.h"

#include "fundamental_types.h"

#include <string>
#include <iostream>

namespace piranha {

    class ThrowRuntimeErrorNode : public Node {
    public:
        ThrowRuntimeErrorNode() {
            /* void */
        }

        ~ThrowRuntimeErrorNode() {
            /* void */
        }

    protected:
        virtual void _evaluate() {
            piranha::native_bool data;
            m_input->fullCompute(&data);

            if (data) {
                throwError("Planned error");
            }
            else {
                // Do nothing
            }
        }

        virtual void registerInputs() {
            registerInput(&m_input, "throw");
        }

        virtual void registerOutputs() {
            /* void */
        }

    protected:
        pNodeInput m_input;
    };

} /* namespace piranha */

#endif /* PIRANHA_THROW_RUNTIME_ERROR_NODE_H */
