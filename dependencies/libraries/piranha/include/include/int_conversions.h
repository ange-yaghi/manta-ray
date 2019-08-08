#ifndef PIRANHA_INT_CONVERSIONS_H
#define PIRANHA_INT_CONVERSIONS_H

#include "fundamental_output.h"

#include "pipe_node.h"

namespace piranha {

    class StringToIntConversion : public StringValueOutput {
    public:
        StringToIntConversion();
        ~StringToIntConversion();

        virtual void fullCompute(void *target) const;
        virtual void registerInputs();

        pNodeInput *getInputConnection() { return &m_input; }

    protected:
        pNodeInput m_input;
    };
    typedef PipeNode<StringToIntConversion> StringToIntConversionNode;

} /* namespace piranha */

#endif /* PIRANHA_INT_CONVERSIONS_H */
