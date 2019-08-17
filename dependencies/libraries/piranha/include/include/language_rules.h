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
        bool isBase = true;

        bool operator==(const NodeTypeConversion &ref) const {
            const NodeTypeConversion *base;
            const NodeTypeConversion *cmp;
            if (isBase) {
                base = this;
                cmp = &ref;
            }
            else {
                base = &ref;
                cmp = this;
            }

            return (cmp->startingType->isCompatibleWith(*base->startingType)) 
                && (cmp->targetType->isCompatibleWith(*base->targetType));
        }
    };

    struct OperatorMapping {
        IrBinaryOperator::OPERATOR op;
        const ChannelType *leftType;
        const ChannelType *rightType;
        bool reversible = true;
        bool isBase = true;

        bool operator==(const OperatorMapping &ref) const {
            const OperatorMapping *base;
            const OperatorMapping *cmp;
            if (isBase) {
                base = this;
                cmp = &ref;
            }
            else {
                base = &ref;
                cmp = this;
            }

            bool typesMatch = cmp->leftType->isCompatibleWith(*base->leftType) 
                && cmp->rightType->isCompatibleWith(*base->rightType);
            if (!typesMatch && base->reversible) {
                typesMatch = cmp->leftType->isCompatibleWith(*base->rightType)
                    && cmp->rightType->isCompatibleWith(*base->leftType);
            }

            return typesMatch && (op == ref.op);
        }
    };

    struct UnaryOperatorMapping {
        IrUnaryOperator::OPERATOR op;
        const ChannelType *type;
        bool isBase = true;

        bool operator==(const UnaryOperatorMapping &ref) const {
            const UnaryOperatorMapping *base;
            const UnaryOperatorMapping *cmp;
            if (isBase) {
                base = this;
                cmp = &ref;
            }
            else {
                base = &ref;
                cmp = this;
            }

            bool typesMatch = cmp->type->isCompatibleWith(*base->type);
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

        void initialize();

        std::string resolveLiteralBuiltinType(LiteralType typeCode) const {
            std::string *builtinType = m_literalRules.lookup(typeCode);
            return (builtinType == nullptr)
                ? ""
                : *builtinType;
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

        bool checkConversion(const ChannelType *input, const ChannelType *output) const;
        Node *generateConversion(const ChannelType *input, const ChannelType *output);
        std::string resolveConversionBuiltinType(const ChannelType *input, const ChannelType *output) const;

        bool checkBuiltinType(const std::string &builtinType) const;

        const Node *getReferenceNode(const std::string &builtinType) const;

    protected:
        virtual void registerBuiltinNodeTypes() = 0;

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
