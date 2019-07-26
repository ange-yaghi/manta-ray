#ifndef PIRANHA_IR_VALUE_H
#define PIRANHA_IR_VALUE_H

#include "ir_parser_structure.h"

#include <vector>

namespace piranha {

    class IrAttribute;
    class NodeOutput;
    class Node;
    class NodeProgram;
    class IrContextTree;

    class IrValue : public IrParserStructure {
    public:
        enum VALUE_TYPE {
            CONSTANT_FLOAT,
            CONSTANT_INT,
            CONSTANT_STRING,
            CONSTANT_LABEL,
            CONSTANT_BOOL,

            BINARY_OPERATION,
            UNARY_OPERATION,

            NODE_REF,
            INTERNAL_REFERENCE
        };

    public:
        IrValue(VALUE_TYPE type);
        virtual ~IrValue();

        VALUE_TYPE getType() const { return m_type; }

    private:
        VALUE_TYPE m_type;
    };

} /* namespace piranha */

#endif /* PIRANHA_IR_VALUE_H */
