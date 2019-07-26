#ifndef PIRANHA_FLOAT_CONVERSIONS_H
#define PIRANHA_FLOAT_CONVERSIONS_H

#include "fundamental_output.h"

#include "pipe_node.h"

namespace piranha {

    class StringToFloatConversionOutput : public FloatValueOutput {
    public:
        StringToFloatConversionOutput();
        ~StringToFloatConversionOutput();

        virtual void fullCompute(void *target) const;
        virtual void registerInputs();

        pNodeInput *getInputConnection() { return &m_input; }

    protected:
        pNodeInput m_input;
    };
    typedef PipeNode<StringToFloatConversionOutput> StringToFloatConversionNode;

    class IntToFloatConversionOutput : public FloatValueOutput {
    public:
        IntToFloatConversionOutput();
        ~IntToFloatConversionOutput();

        virtual void fullCompute(void *target) const;
        virtual void registerInputs();

        pNodeInput *getInputConnection() { return &m_input; }

    protected:
        pNodeInput m_input;
    };
    typedef PipeNode<IntToFloatConversionOutput> IntToFloatConversionNode;

} /* namespace piranha */

#endif /* PIRANHA_FLOAT_CONVERSIONS_H */
