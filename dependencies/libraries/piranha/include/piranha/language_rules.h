#ifndef PIRANHA_LANGUAGE_RULES_H
#define PIRANHA_LANGUAGE_RULES_H

#include "literal_node.h"
#include "ir_binary_operator.h"
#include "ir_value.h"
#include "rule.h"
#include "operation_node.h"
#include "key_value_lookup.h"
#include "channel_type.h"
#include "ir_unary_operator.h"

#include <string>
#include <vector>

namespace piranha {

    class Node;
    class NodeProgram;
    class IrNode;
    class IrContextTree;
    class IrParserStructure;
    class IrBinaryOperator;

    struct NodeTypeConversion {
        const ChannelType *startingType;
        const ChannelType *targetType;

        bool operator==(const NodeTypeConversion &ref) const {
            return (startingType == ref.startingType) && (targetType == ref.targetType);
        }
    };

    struct OperatorMapping {
        IrBinaryOperator::OPERATOR op;
        const ChannelType *leftType;
        const ChannelType *rightType;
        bool reversible = true;

        bool operator==(const OperatorMapping &ref) const {
            bool typesMatch = leftType == ref.leftType && rightType == ref.rightType;
            if (!typesMatch && ref.reversible && reversible) {
                typesMatch = (leftType == ref.rightType && rightType == ref.leftType);
            }

            return typesMatch && (op == ref.op);
        }
    };

    struct UnaryOperatorMapping {
        IrUnaryOperator::OPERATOR op;
        const ChannelType *type;

        bool operator==(const UnaryOperatorMapping &ref) const {
            bool typesMatch = type == ref.type;
            return typesMatch && (op == ref.op);
        }
    };

    struct BuiltinTypeInfo {
        const ChannelType *nodeType;
    };

    struct NullType {};
    typedef Rule<BuiltinTypeInfo, Node> BuiltinRule;

    class LanguageRules {
    public:
        LanguageRules();
        ~LanguageRules();

        virtual void registerBuiltinNodeTypes() = 0;

        template <typename NativeType>
        LiteralNode<NativeType> *generateLiteral(const NativeType &data) const {
            typedef LiteralNode<NativeType> ResolvedLiteralNode;

            LiteralType literalType = LiteralTypeLookup<NativeType>();

            std::string *builtinType = m_literalRules.lookup(literalType);
            if (builtinType == nullptr) return nullptr;

            ResolvedLiteralNode *node = static_cast<ResolvedLiteralNode *>(generateBuiltinType(*builtinType));
            node->setData(data);
            
            return node;
        }

        Node *generateNode(const std::string &builtinName) const;
        std::string resolveOperatorBuiltinType(
            IrBinaryOperator::OPERATOR op, const ChannelType *left, const ChannelType *right) const;
        std::string resolveUnaryOperatorBuiltinType(
            IrUnaryOperator::OPERATOR op, const ChannelType *type) const;
        const ChannelType *resolveChannelType(const std::string &builtinName) const;

        template <typename NativeType>
        std::string getLiteralBuiltinName() const { return ""; }
        template<> std::string getLiteralBuiltinName<piranha::native_bool>() const { return *m_literalRules.lookup(LITERAL_BOOL); }
        template<> std::string getLiteralBuiltinName<piranha::native_string>() const  { return *m_literalRules.lookup(LITERAL_STRING); }
        template<> std::string getLiteralBuiltinName<piranha::native_int>() const  { return *m_literalRules.lookup(LITERAL_INT); }
        template<> std::string getLiteralBuiltinName<piranha::native_float>() const { return *m_literalRules.lookup(LITERAL_FLOAT); }

        bool checkConversion(const NodeTypeConversion &conversion) const;
        Node *generateConversion(const NodeTypeConversion &conversion);
        std::string resolveConversionBuiltinType(const NodeTypeConversion &conversion) const;

        bool checkBuiltinType(const std::string &builtinType) const;

        const Node *getReferenceNode(const std::string &builtinType) const;

    protected:
        Node *generateOperator(IrBinaryOperator::OPERATOR op, const ChannelType *left, const ChannelType *right);
        Node *generateUnaryOperator(IrUnaryOperator::OPERATOR op, const ChannelType *type);

    protected:
        Node *generateBuiltinType(const std::string &typeName) const;

        template <typename NodeType>
        void registerBuiltinType(const std::string &builtinName, const ChannelType *nodeType = nullptr) {
            BuiltinRule *newRule =
                m_builtinRules.newValue<SpecializedRule<BuiltinTypeInfo, NodeType, Node>>(builtinName);
            newRule->setValue({ nodeType });
        }

        void registerLiteralType(LiteralType literalType, const std::string &builtinType);

        void registerConversion(const NodeTypeConversion &conversion, const std::string &builtinType);
        void registerOperator(const OperatorMapping &op, const std::string &builtinType);
        void registerUnaryOperator(const UnaryOperatorMapping &op, const std::string &builtinType);

    public:
        template <typename BaseType>
        LiteralNode<BaseType> *generateLiteralInternal() { return nullptr; }

    private:
        KeyValueLookup<std::string, BuiltinRule> m_builtinRules;
        KeyValueLookup<NodeTypeConversion, std::string> m_conversionRules;
        KeyValueLookup<OperatorMapping, std::string> m_operatorRules;
        KeyValueLookup<UnaryOperatorMapping, std::string> m_unaryOperatorRules;
        KeyValueLookup<LiteralType, std::string> m_literalRules;

        NodeProgram *m_nodeProgram;
    };

} /* namespace piranha */

#endif /* PIRANHA_LANGUAGE_RULES_H */
