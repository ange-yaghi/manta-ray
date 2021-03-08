#ifndef CONSOLE_LOG_NODE_H
#define CONSOLE_LOG_NODE_H

#include <piranha.h>

#include "session.h"
#include "console.h"

namespace manta {

    class ConsoleOutputNode : public piranha::Node {
    public:
        ConsoleOutputNode() {
            m_dataOut = nullptr;
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

            Session::get().getConsole()->out(data);
        }

        virtual void registerInputs() {
            registerInput(&m_dataOut, "data");
        }

        virtual void registerOutputs() {
            /* void */
        }

    protected:
        piranha::pNodeInput m_dataOut;
    };

} /* namespace manta */

#endif /* CONSOLE_LOG_NODE_H */
