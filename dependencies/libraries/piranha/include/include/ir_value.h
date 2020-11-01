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
        enum class ValueType {
            ConstantFloat,
            ConstantInt,
            ConstantString,
            ConstantLabel,
            ConstantBool,

            BinaryOperation,
            UnaryOperation,

            NodeReference,
            InternalReference
        };

    public:
        IrValue(ValueType type);
        virtual ~IrValue();

        ValueType getType() const { return m_type; }

    private:
        ValueType m_type;
    };

} /* namespace piranha */

#endif /* PIRANHA_IR_VALUE_H */
