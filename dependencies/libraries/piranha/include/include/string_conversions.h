#ifndef PIRANHA_STRING_CONVERSIONS_H
#define PIRANHA_STRING_CONVERSIONS_H

#include "fundamental_output.h"

#include "pipe_node.h"

namespace piranha {

    class FloatToStringConversionOutput : public StringValueOutput {
    public:
        FloatToStringConversionOutput();
        ~FloatToStringConversionOutput();

        virtual void fullCompute(void *target) const;
        virtual void registerInputs();

        pNodeInput *getInputConnection() { return &m_input; }

    protected:
        pNodeInput m_input;
    };
    typedef PipeNode<FloatToStringConversionOutput> FloatToStringConversionNode;

} /* namespace piranha */

#endif /* PIRANHA_STRING_CONVERSIONS_H */
