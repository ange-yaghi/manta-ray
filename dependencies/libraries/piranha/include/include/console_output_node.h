#ifndef PIRANHA_CONSOLE_OUTPUT_NODE_H
#define PIRANHA_CONSOLE_OUTPUT_NODE_H

#include "node.h"

#include "fundamental_types.h"

#include <string>
#include <iostream>

namespace piranha {

    class ConsoleOutputNode : public Node {
    public:
        ConsoleOutputNode() {
            /* void */
        }

        ~ConsoleOutputNode() {
            /* void */
        }

    protected:
        virtual void _initialize() {
            m_dataOut = nullptr;
        }

        virtual void _evaluate() {
            std::string data;
            m_dataOut->fullCompute(&data);

            std::cout << data;
        }

        virtual void registerInputs() {
            registerInput(&m_dataOut, "data");
        }

        virtual void registerOutputs() {
            /* void */
        }

    protected:
        pNodeInput m_dataOut;
    };

} /* namespace piranha */

#endif /* PIRANHA_CONSOLE_OUTPUT_NODE_H */
