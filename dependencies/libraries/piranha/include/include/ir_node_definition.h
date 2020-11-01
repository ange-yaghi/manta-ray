#ifndef PIRANHA_IR_NODE_DEFINITION_H
#define PIRANHA_IR_NODE_DEFINITION_H

#include "ir_parser_structure.h"

#include "ir_token_info.h"
#include "ir_structure_list.h"
#include "fundamental_types.h"

namespace piranha {

    class IrAttributeDefinitionList;
    class IrAttributeDefinition;
    class IrCompilationUnit;

    class IrNodeDefinition : public IrParserStructure {
    public:
        IrNodeDefinition(const IrTokenInfo_string &name);
        virtual ~IrNodeDefinition();

        const IrTokenInfo *getBuiltinNameToken() const { return m_definesBuiltin ? &m_builtinName : nullptr; }
        std::string getBuiltinName() const { return m_builtinName.data; }
        void setBuiltinName(const IrTokenInfo_string &builtin);

        void setDefinesBuiltin(bool defines) { m_definesBuiltin = defines; }
        bool isBuiltin() const { return m_definesBuiltin; }

        void setIsInline(bool isInline) { m_isInline = isInline; }
        bool isInline() const { return m_isInline; }

        const IrTokenInfo_string *getNameToken() const { return &m_name; }
        std::string getName() const { return m_name.data; }

        void setAttributeDefinitionList(IrAttributeDefinitionList *definitions);
        const IrAttributeDefinitionList *getAttributeDefinitionList() const { return m_attributes; }

        void setScopeToken(const IrTokenInfo_string &token);
        const IrTokenInfo *getScopeToken() const { return &m_scopeToken; }

        IrAttributeDefinition *getAttributeDefinition(const std::string &attributeName) const;

        void setBody(IrNodeList *body) { m_body = body; registerComponent(body); }
        IrNodeList *getBody() const { return m_body; }

        virtual IrParserStructure *resolveName(const std::string &name) const;

        const ChannelType *getChannelType();
        IrNodeDefinition *getAliasType();

        virtual void free();

    protected:
        IrTokenInfo_string m_name;

        bool m_definesBuiltin;
        IrTokenInfo_string m_builtinName;

        bool m_isInline;

        IrAttributeDefinitionList *m_attributes;
        IrNodeList *m_body;

        IrTokenInfo_string m_scopeToken;

        // Resolution stage
    public:
        virtual void checkCircularDefinitions();

        int countSymbolIncidence(const std::string &name) const;
        IrParserStructure *resolveLocalName(const std::string &name) const;

    protected:
        virtual void _validate();
        void validateBuiltinMappings();
    };

} /* namespace piranha */

#endif /* PIRANHA_IR_NODE_DEFINITION_H */
