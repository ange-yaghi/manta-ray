#ifndef PIRANHA_IR_NODE_H
#define PIRANHA_IR_NODE_H

#include "ir_parser_structure.h"

#include "ir_token_info.h"

#include <vector>
#include <fstream>

namespace piranha {

    class IrAttributeList;
    class IrNodeDefinition;
    class IrCompilationUnit;
    class IrAttribute;
    class IrAttributeDefinition;
    class IrContextTree;
    class Node;
    class NodeProgram;

    class IrNode : public IrParserStructure {
    public:
        IrNode();
        IrNode(const IrTokenInfo_string &type, const IrTokenInfo_string &name, IrAttributeList *attributes, 
            const IrTokenInfo_string &library);
        IrNode(const IrTokenInfo_string &type, IrAttributeList *attributes, 
            const IrTokenInfo_string &library);
        IrNode(const IrTokenInfo_string &name, IrNodeDefinition *nodeDefinition, IrAttributeList *attributes);
        ~IrNode();

        IrTokenInfo getTypeToken() const { return m_type; }
        IrTokenInfo getNameToken() const { return m_name; }

        void setName(const std::string &name) { m_name.data = name; }

        const std::string &getType() const { return m_type.data; }
        const std::string &getName() const { return m_name.data; }

        virtual const ChannelType *getImmediateChannelType();

        void setAttributes(IrAttributeList *list);
        IrAttributeList *getAttributes() const { return m_attributes; }
        IrAttribute *getAttribute(const std::string &name, int *count = nullptr) const;

        virtual void setScopeParent(IrParserStructure *parentScope);

        IrParserStructure *getDefaultPort(bool *failed);
        virtual IrNode *getAsNode() { return this; }

        void writeTraceToFile(std::ofstream &file);

        virtual IrParserStructure *getImmediateReference(
            const IrReferenceQuery &query, IrReferenceInfo *output = nullptr);
        virtual void checkReferences(IrContextTree *inputContext = nullptr);

        void setInterface(bool isInterface) { m_isInterface = isInterface; }
        bool isInterface() const { return m_isInterface; }

        void setBuild(bool enableBuild) { m_build = enableBuild; }
        bool isBuildEnabled() const { return m_build; }

        void setThis(IrValue *memberThis);

        virtual void free();

    protected:
        IrTokenInfo_string m_type;
        IrTokenInfo_string m_name;
        IrTokenInfo_string m_library;

        IrAttributeList *m_attributes;

        IrAttribute *getAttribute(IrAttributeDefinition *definition, int *count = nullptr) const;

        // Resolution stage
    public:
        IrNodeDefinition *getDefinition() const { return m_definition; }
        void setDefinition(IrNodeDefinition *definition) { m_definition = definition; }

        virtual IrParserStructure *resolveLocalName(const std::string &name) const;
        virtual void _checkCircularDefinitions(IrContextTree *context, IrNodeDefinition *root);

    protected:
        virtual void _resolveDefinitions();
        virtual void _validate();
        virtual void _checkInstantiation();
        virtual void _expand();
        virtual void _expand(IrContextTree *context);
        virtual void _checkTypes();
        virtual void _checkTypes(IrContextTree *context);

        void resolveNodeDefinition();
        void resolveAttributeDefinitions();

        IrNodeDefinition *m_definition;
        bool m_isInterface;
        bool m_build;

    public:
        virtual Node *_generateNode(IrContextTree *context, NodeProgram *program, NodeContainer *container);
        virtual NodeOutput *_generateNodeOutput(IrContextTree *context, NodeProgram *program, NodeContainer *container);
    };

} /* namespace piranha */

#endif /* PIRANHA_IR_NODE_H */
