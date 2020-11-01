#ifndef PIRANHA_IR_ATTRIBUTE_DEFINITION_H
#define PIRANHA_IR_ATTRIBUTE_DEFINITION_H

#include "ir_parser_structure.h"

#include "ir_token_info.h"

namespace piranha {

    class IrValue;
    class IrInputConnection;
    class CompilationError;
    class IrNodeDefinition;

    class IrAttributeDefinition : public IrParserStructure {
    public:
        enum class Direction {
            Input,
            Output,
            Modify,
            Toggle
        };

    public:
        IrAttributeDefinition(const IrTokenInfo_string &directionToken, 
            const IrTokenInfo_string &name, Direction dir);
        IrAttributeDefinition(const IrTokenInfo_string &name);
        virtual ~IrAttributeDefinition();

        const IrTokenInfo *getNameToken() const { return &m_name; }
        std::string getName() const { return m_name.data; }

        void setDefaultValue(IrValue *value);
        IrValue *getDefaultValue() const { return m_defaultValue; }

        void setAliasToken(const IrTokenInfo_string &name);
        const IrTokenInfo *getAliasToken() const { return (m_isAlias) ? &m_aliasToken : nullptr; }
        void setAlias(bool isAlias) { m_isAlias = isAlias; }
        bool isAlias() const { return m_isAlias; }

        const IrTokenInfo *getDirectionToken() const { return &m_directionToken; }
        void setDirection(Direction direction) { m_direction = direction; }
        Direction getDirection() const { return m_direction; }

        virtual IrParserStructure *getImmediateReference(const IrReferenceQuery &query, IrReferenceInfo *output);

        void setTypeInfo(const IrTokenInfoSet<std::string, 2> &typeInfo) { m_typeInfo = typeInfo; }
        bool typeInfoSpecified() const { return m_typeInfo.data[0].specified; }

        IrNodeDefinition *getTypeDefinition() const;
        IrNodeDefinition *getImmediateTypeDefinition() const { return m_typeDefinition; }
        virtual const ChannelType *getImmediateChannelType();

        virtual void _expand(IrContextTree *context);
        virtual void _checkTypes(IrContextTree *context);

        bool isInput() const;
        bool isThis() const;

    protected:
        IrTokenInfo_string m_directionToken;
        IrTokenInfo_string m_aliasToken;
        IrTokenInfo_string m_name;
        IrTokenInfoSet<std::string, 2> m_typeInfo;

        IrValue *m_defaultValue;

        Direction m_direction;
        bool m_isAlias;

        std::vector<IrInputConnection *> m_impliedMembers;

        // Resolution stage
    protected:
        virtual void _resolveDefinitions();

        IrNodeDefinition *m_typeDefinition;

    protected:
        virtual Node *_generateNode(IrContextTree *context, NodeProgram *program, NodeContainer *container);
        virtual NodeOutput *_generateNodeOutput(IrContextTree *context, NodeProgram *program, NodeContainer *container);
    };

} /* namespace piranha */

#endif /* PIRANHA_IR_ATTRIBUTE_DEFINITION_H */
