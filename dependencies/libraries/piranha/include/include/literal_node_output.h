#ifndef PIRANHA_LITERAL_NODE_OUTPUT_H
#define PIRANHA_LITERAL_NODE_OUTPUT_H

#include "fundamental_output.h"

#include "fundamental_types.h"

#include <string>

namespace piranha {

    template <typename LiteralType>
    class LiteralNodeOutput : public FundamentalOutput<LiteralType> {
    public:
        LiteralNodeOutput() {
            /* void */
        }

        virtual ~LiteralNodeOutput() {
            /* void */
        }

        void setData(const LiteralType &data) {
            m_data = data;
        }

        LiteralType getData() const {
            return m_data;
        }

        virtual void fullCompute(void *_target) const {
            LiteralType *target = reinterpret_cast<LiteralType *>(_target);
            *target = m_data;
        }

    protected:
        LiteralType m_data;
    };

    typedef LiteralNodeOutput<piranha::native_float> LiteralFloatOutput;
    typedef LiteralNodeOutput<piranha::native_int> LiteralIntOutput;
    typedef LiteralNodeOutput<piranha::native_string> LiteralStringOutput;
    typedef LiteralNodeOutput<piranha::native_bool> LiteralBoolOutput;

} /* namespace piranha */

#endif /* PIRANHA_LITERAL_NODE_OUTPUT_H */
