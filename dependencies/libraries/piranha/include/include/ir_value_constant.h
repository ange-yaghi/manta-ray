#ifndef PIRANHA_IR_VALUE_CONSTANT_H
#define PIRANHA_IR_VALUE_CONSTANT_H

#include "ir_value.h"

#include "ir_token_info.h"
#include "compilation_error.h"
#include "ir_compilation_unit.h"
#include "ir_literal_node.h"
#include "ir_context_tree.h"
#include "ir_attribute_list.h"
#include "standard_allocator.h"
#include "node_program.h"
#include "language_rules.h"
#include "node.h"
#include "literal_node.h"
#include "fundamental_types.h"

namespace piranha {

    class IrNode;

    template <typename T, IrValue::VALUE_TYPE TypeCode>
    class IrValueConstant : public IrValue {
    protected:
        typedef T_IrTokenInfo<T> _TokenInfo;

    public:
        IrValueConstant(const _TokenInfo &value) : IrValue(TypeCode) { 
            m_value = value.data; useToken(value); 
        }

        virtual ~IrValueConstant() { /* void */ }

        const _TokenInfo *getToken() const { return &m_token; }
        void useToken(const _TokenInfo &info) { m_value = info.data; registerToken(&info); }

        virtual void setValue(const T &value) { m_value = value; }
        T getValue() const { return m_value; }

    protected:
        virtual void _validate() {
            m_value = validateData(m_value, true);
        }

        template <typename _T>
        const _T validateData(const _T &data, bool recordErrors = false) { return data; }

        template <>
        const piranha::native_string validateData<piranha::native_string>(const piranha::native_string &data, bool recordErrors) {
            piranha::native_string res;
            piranha::native_string::const_iterator it = data.begin();
            while (it != data.end()) {
                char c = *it++;
                if (c == '\\' && it != data.end()) {
                    switch (*it++) {
                    case '\\': 
                        c = '\\'; break;
                    case 'n': 
                        c = '\n'; break;
                    case 't': c = '\t'; break;
                    default:
                        // Invalid escape sequence
                        continue;
                    }
                }
                res += c;
            }

            return res;
        }

        std::string valueToString() const {
            std::stringstream ss;
            ss << m_value;

            return ss.str();
        }

    protected:
        T m_value;
        _TokenInfo m_token;
    };

    template <typename T, IrValue::VALUE_TYPE TypeCode>
    class IrValueLiteral : public IrValueConstant<T, TypeCode> {
    protected:
        typedef IrValueConstant<T, TypeCode> Base;

    public:
        IrValueLiteral(const Base::_TokenInfo &value) : Base(value) {}
        ~IrValueLiteral() {}

        virtual void _expand(IrContextTree *context) {
            if (Base::m_rules == nullptr) return;

            std::string builtinType =
                Base::m_rules->resolveLiteralBuiltinType(LiteralTypeLookup<T>());

            if (builtinType.empty()) {
                // TODO: raise error, literal not supported
            }

            int count = 0;
            IrCompilationUnit *parentUnit = Base::getParentUnit();
            IrNodeDefinition *nodeDefinition = parentUnit->resolveBuiltinNodeDefinition(builtinType, &count);

            if (nodeDefinition == nullptr) {
                // TODO: raise error, literal type not defined or not available
            }

            // Generate the expansion
            IrAttributeList *attributeList = new IrAttributeList();

            IrLiteralNode<T> *expansion = new IrLiteralNode<T>();
            expansion->setName("\"" + Base::valueToString() + "\"");
            expansion->setLiteralData(Base::template validateData<T>(Base::m_value));
            expansion->setAttributes(attributeList);
            expansion->setLogicalParent(this);
            expansion->setScopeParent(this);
            expansion->setDefinition(nodeDefinition);
            expansion->setRules(Base::m_rules);
            expansion->resolveDefinitions();
            expansion->expand(context);

            if (nodeDefinition == nullptr) {
                // TODO: raise error here
            }

            *Base::m_expansions.newValue(context) = expansion;
        }

        virtual IrParserStructure *getImmediateReference(
            const Base::IrReferenceQuery &query, Base::IrReferenceInfo *output) 
        {
            IR_RESET(query);
            if (Base::m_rules == nullptr) return nullptr;

            IrNode **pNode = Base::m_expansions.lookup(query.inputContext);

            if (pNode == nullptr) {
                IR_DEAD_END();
                return nullptr;
            }
            else if (*pNode == nullptr) {
                IR_FAIL();
                return nullptr;
            }
            else return *pNode;
        }
    };

    // Specialized type for labels
    class IrValueLabel : public IrValueConstant<std::string, IrValue::CONSTANT_LABEL> {
    public:
        IrValueLabel(const _TokenInfo &value) : IrValueConstant(value) { /* void */ }
        ~IrValueLabel() { /* void */ }

        virtual IrParserStructure *
            getImmediateReference(const IrReferenceQuery &query, IrReferenceInfo *output) 
        {
            IR_RESET(query);

            IrParserStructure *reference = resolveName(m_value);

            if (reference == nullptr) {
                // Try searching for a global variable
                // TODO: if nested node definitions are ever introduced, then some more sophisticated
                // logic would be needed here to resolve the actual context. For now we can assume
                // that all global variables are defined at global (root) scope
                reference = getParentUnit()->resolveLocalName(m_value);
                IR_INFO_OUT(newContext, query.inputContext->getRoot());
            }

            // Do error checking
            if (reference == nullptr) {
                IR_FAIL();

                if (query.recordErrors && IR_EMPTY_CONTEXT()) {
                    IR_ERR_OUT(new CompilationError(m_summaryToken,
                        ErrorCode::UnresolvedReference, query.inputContext));
                }

                return nullptr;
            }    

            return reference;
        }

        virtual Node *_generateNode(IrContextTree *context, NodeProgram *program, NodeContainer *container) {
            return IrParserStructure::_generateNode(context, program, container);
        }

        virtual NodeOutput *_generateNodeOutput(IrContextTree *context, NodeProgram *program, NodeContainer *container) {
            return IrParserStructure::_generateNodeOutput(context, program, container);
        }

        virtual void _expand(IrContextTree *context) {
            IrParserStructure::_expand(context);
        }
    };

    // Specialized type for node references
    class IrValueNodeRef : public IrValueConstant<IrNode *, IrValue::NODE_REF> {
    public:
        IrValueNodeRef(const _TokenInfo &value) : IrValueConstant(value) { 
            registerComponent(value.data); 
        }

        ~IrValueNodeRef() { /* void */ }

        virtual void setValue(IrNode *const &value) {
            m_value = value; 
            registerComponent(value); 
        }

        virtual IrParserStructure *
            getImmediateReference(const IrReferenceQuery &query, IrReferenceInfo *output) 
        {
            IR_RESET(query);
            return m_value;
        }

        virtual IrNode *getAsNode() {
            return m_value;
        }

        virtual Node *_generateNode(IrContextTree *context, NodeProgram *program, NodeContainer *container) {
            return IrParserStructure::_generateNode(context, program, container);
        }

        virtual NodeOutput *_generateNodeOutput(IrContextTree *context, NodeProgram *program, NodeContainer *container) {
            return IrParserStructure::_generateNodeOutput(context, program, container);
        }

        virtual void _expand(IrContextTree *context) {
            IrParserStructure::_expand(context);
        }
    };

    // Specialized type for internal structure references (during expansions)
    class IrInternalReference 
        : public IrValueConstant<IrParserStructure *, IrValue::INTERNAL_REFERENCE> 
    {
    public:
        IrInternalReference(IrParserStructure *reference, IrContextTree *newContext) 
            : IrValueConstant(_TokenInfo()) 
        { 
            setValue(reference); 
            m_newContext = newContext; 
        }

        ~IrInternalReference() { /* void */ }

        virtual void setValue(IrParserStructure *const &value) {
            m_value = value;
        }

        IrContextTree *getNewContext() const { return m_newContext; }

        virtual IrParserStructure *
            getImmediateReference(const IrReferenceQuery &query, IrReferenceInfo *output) 
        {
            IR_RESET(query);

            IR_INFO_OUT(newContext, m_newContext);
            return m_value;
        }

        virtual Node *_generateNode(IrContextTree *context, NodeProgram *program, NodeContainer *container) {
            return IrParserStructure::_generateNode(context, program, container);
        }

        virtual NodeOutput *_generateNodeOutput(IrContextTree *context, NodeProgram *program, NodeContainer *container) {
            return IrParserStructure::_generateNodeOutput(context, program, container);
        }

        virtual void _expand(IrContextTree *context) {
            IrParserStructure::_expand(context);
        }

    protected:
        IrContextTree *m_newContext;
    };

    typedef IrValueLiteral<piranha::native_int, IrValue::CONSTANT_INT> IrValueInt;
    typedef IrValueLiteral<piranha::native_string, IrValue::CONSTANT_STRING> IrValueString;
    typedef IrValueLiteral<piranha::native_float, IrValue::CONSTANT_FLOAT> IrValueFloat;
    typedef IrValueLiteral<piranha::native_bool, IrValue::CONSTANT_BOOL> IrValueBool;

} /* namespace piranha */

#endif /* PIRANHA_IR_VALUE_CONSTANT_H */
