#ifndef PIRANHA_FUNDAMENTAL_OUTPUT_H
#define PIRANHA_FUNDAMENTAL_OUTPUT_H

#include "node_output.h"

#include "fundamental_types.h"

#include <string>

namespace piranha {

    template <typename FundamentalType>
    class FundamentalOutput : public NodeOutput {};

    template <>
    class FundamentalOutput<piranha::native_float> : public NodeOutput {
    public:
        FundamentalOutput() : NodeOutput(&FundamentalType::FloatType) {};
        ~FundamentalOutput() { /* void */ }
    };

    template <>
    class FundamentalOutput<piranha::native_int> : public NodeOutput {
    public:
        FundamentalOutput() : NodeOutput(&FundamentalType::IntType) {};
        ~FundamentalOutput() { /* void */ }
    };

    template <>
    class FundamentalOutput<piranha::native_bool> : public NodeOutput {
    public:
        FundamentalOutput() : NodeOutput(&FundamentalType::BoolType) {};
        ~FundamentalOutput() { /* void */ }
    };

    template <>
    class FundamentalOutput<piranha::native_string> : public NodeOutput {
    public:
        FundamentalOutput() : NodeOutput(&FundamentalType::StringType) {};
        ~FundamentalOutput() { /* void */ }
    };

    typedef FundamentalOutput<piranha::native_float> FloatValueOutput;
    typedef FundamentalOutput<piranha::native_int> IntValueOutput;
    typedef FundamentalOutput<piranha::native_bool> BoolValueOutput;
    typedef FundamentalOutput<piranha::native_string> StringValueOutput;

} /* namespace piranha */

#endif /* PIRANHA_FUNDAMENTAL_OUTPUT_H */
