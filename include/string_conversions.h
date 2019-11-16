#ifndef MANTARAY_STRING_CONVERSIONS_H
#define MANTARAY_STRING_CONVERSIONS_H

#include <piranha.h>

namespace manta {

    class VectorToStringConversionOutput : public piranha::StringValueOutput {
    public:
        VectorToStringConversionOutput();
        ~VectorToStringConversionOutput();

        virtual void fullCompute(void *target) const;
        virtual void registerInputs();

        piranha::pNodeInput *getInputConnection() { return &m_input; }

    protected:
        piranha::pNodeInput m_input;
    };
    typedef piranha::PipeNode<VectorToStringConversionOutput> VectorToStringConversionNode;

} /* namespace manta */

#endif /* MANTARAY_STRING_CONVERSIONS_H */
