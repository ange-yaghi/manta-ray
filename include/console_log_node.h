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
            m_color = nullptr;
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

            math::Vector color;
            static_cast<VectorNodeOutput *>(m_color)->sample(nullptr, (void *)&color);

            Session::get().getConsole()->out(data, color);
        }

        virtual void registerInputs() {
            registerInput(&m_dataOut, "data");
            registerInput(&m_color, "color");
        }

        virtual void registerOutputs() {
            /* void */
        }

    protected:
        piranha::pNodeInput m_dataOut;
        piranha::pNodeInput m_color;
    };

} /* namespace manta */

#endif /* CONSOLE_LOG_NODE_H */
