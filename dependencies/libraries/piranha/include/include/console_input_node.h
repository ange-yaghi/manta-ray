#ifndef PIRANHA_CONSOLE_INPUT_NODE_H
#define PIRANHA_CONSOLE_INPUT_NODE_H

#include "node.h"

#include "literal_node_output.h"
#include "fundamental_types.h"

#include <string>
#include <iostream>

namespace piranha {

    class ConsoleInputNode : public Node {
    public:
        ConsoleInputNode() {
            /* void */
        }

        ~ConsoleInputNode() {
            /* void */
        }

    protected:
        virtual void _initialize() {
            m_promptInput = nullptr;
        }

        virtual void _evaluate() {
            std::string prompt;
            m_promptInput->fullCompute(&prompt);

            std::cout << prompt;
            std::cin >> m_inputData;

            m_output.setData(m_inputData);
        }

        virtual void registerInputs() {
            registerInput(&m_promptInput, "prompt");
        }

        virtual void registerOutputs() {
            setPrimaryOutput("raw_input");
            registerOutput(&m_output, "raw_input");
        }

    protected:
        pNodeInput m_promptInput;
        LiteralNodeOutput<piranha::native_string> m_output;
        std::string m_inputData;
    };

} /* namespace piranha */

#endif /* PIRANHA_CONSOLE_OUTPUT_NODE_H */
