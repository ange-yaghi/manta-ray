#ifndef PIRANHA_IR_PARSER_STRUCTURE_H
#define PIRANHA_IR_PARSER_STRUCTURE_H

#include "ir_token_info.h"
#include "ir_visibility.h"
#include "pkey_value_lookup.h"
#include "ir_context_tree.h"

#include <vector>
#include <fstream>

#define IR_RESET(query)                 { if (output != nullptr) { output->reset(); output->newContext = (query).inputContext; } }
#define IR_INFO_OUT(param, data)        { if (output != nullptr) { output->param = (data); } }
#define IR_ERR_OUT(data)                IR_INFO_OUT(err, (data)); 
#define IR_FAIL()                       IR_INFO_OUT(failed, true);
#define IR_DEAD_END()                   IR_INFO_OUT(reachedDeadEnd, true);
#define IR_FAILED(output)               (((output) != nullptr) ? (output)->failed : false)
#define IR_EMPTY_CONTEXT()              (query.inputContext == nullptr || query.inputContext->isEmpty())

namespace piranha {

    class IrValue;
    class IrCompilationUnit;
    class CompilationError;
    class IrNode;
    class IrNodeDefinition;
    class Node;
    class NodeOutput;
    class NodeProgram;
    class ChannelType;
    class LanguageRules;
    class NodeContainer;

    class IrParserStructure {
    public:
        struct IrReferenceQuery {
            IrReferenceQuery();
            ~IrReferenceQuery();

            // Inputs
            IrContextTree *inputContext;
            bool recordErrors;
            bool recordInfiniteLoops;
        };

        struct IrReferenceChain {
            struct Link {
                IrParserStructure *structure;
                IrContextTree *context;
            };

            void addLink(IrParserStructure *structure, IrContextTree *context);
            int searchLink(IrParserStructure *structure, IrContextTree *context);

            std::vector<Link> list;
        };

        struct IrReferenceInfo {
            IrReferenceInfo();
            ~IrReferenceInfo();

            void reset();

            // Ouputs
            IrContextTree *newContext;
            CompilationError *err;

            bool failed;
            bool reachedDeadEnd;
            bool touchedMainContext;
            int infiniteLoop;
            IrNodeDefinition *fixedType;
            IrContextTree *fixedTypeContext;
            bool staticType;

            bool isFixedType() const { return fixedType != nullptr; }
            bool isFixedTypeOutside(const IrContextTree *context) const { 
                return isFixedType() && fixedTypeContext->isOutside(context); 
            }
            bool isStaticType() const { return isFixedType() && staticType; }
        };

    public:
        IrParserStructure();
        virtual ~IrParserStructure();
        
        void setRules(const LanguageRules *rules);
        const LanguageRules *getRules() const { return m_rules; }

        const IrTokenInfo *getSummaryToken() const { return &m_summaryToken; }
        void registerToken(const IrTokenInfo *tokenInfo);

        void registerComponent(IrParserStructure *child);
        int getComponentCount() const { return (int)m_components.size(); }

        virtual void setScopeParent(IrParserStructure *parent) { m_scopeParent = parent; }
        IrParserStructure *getScopeParent() const { return m_scopeParent; }

        virtual void setLogicalParent(IrParserStructure *parent) { m_logicalParent = parent; }
        IrParserStructure *getLogicalParent() const { return m_logicalParent; }

        virtual IrParserStructure *resolveName(const std::string &name) const;
        virtual IrParserStructure *resolveLocalName(const std::string &name) const;

        bool getDefinitionsResolved() const { return m_definitionsResolved; }
        bool isValidated() const { return m_validated; }
        virtual const ChannelType *getImmediateChannelType() { return nullptr; }
        virtual IrParserStructure *getImmediateReference(
            const IrReferenceQuery &query, IrReferenceInfo *output = nullptr);
        IrParserStructure *getReference(const IrReferenceQuery &query, IrReferenceInfo *output = nullptr);
        IrParserStructure *getReference(const IrReferenceQuery &query, IrReferenceInfo *output, IrReferenceChain *chain);

        bool allowsExternalAccess() const;

        void setVisibility(IrVisibility visibility) { m_visibility = visibility; }
        IrVisibility getVisibility() const { return m_visibility; }

        void setDefaultVisibility(IrVisibility visibility) { m_defaultVisibility = visibility; }
        IrVisibility getDefaultVisibility() const { return m_defaultVisibility; }

        void setCheckReferences(bool check) { m_checkReferences = check; }
        bool getCheckReferences() const { return m_checkReferences; }

        void setParentUnit(IrCompilationUnit *unit) { m_parentUnit = unit; }
        IrCompilationUnit *getParentUnit() const;

        virtual IrNode *getAsNode() { return nullptr; }

        bool isInfiniteLoop(IrContextTree *context);
        void setInfiniteLoop(IrContextTree *context);

        virtual void free();

    public:
        // Compilation stages
        void resolveDefinitions();
        virtual void checkCircularDefinitions();
        void checkCircularDefinitions(IrContextTree *context, IrNodeDefinition *root);
        void expand();
        void checkInstantiation();
        void checkTypes();
        void validate();

        void expand(IrContextTree *context);
        void expandChain(IrContextTree *context);
        void expandChain(IrContextTree *context, IrReferenceChain *chain);
        virtual void checkReferences(IrContextTree *inputContext = nullptr);
        void checkTypes(IrContextTree *inputContext);

    protected:
        virtual void _checkCircularDefinitions(IrContextTree *context, IrNodeDefinition *root);
        virtual void _resolveDefinitions();
        virtual void _expand();
        virtual void _checkInstantiation();
        virtual void _validate();

        virtual void _checkTypes();
        virtual void _checkTypes(IrContextTree *context);

        virtual void _expand(IrContextTree *context);

        void addTree(IrContextTree *tree) { m_trees.push_back(tree); }

    protected:
        IrCompilationUnit *m_parentUnit;
        IrParserStructure *m_scopeParent;
        IrParserStructure *m_logicalParent;
        IrTokenInfo m_summaryToken;

        PKeyValueLookup<IrContextTree, IrNode *> m_expansions;
        std::vector<IrContextTree *> m_infiniteLoops;

        std::vector<IrContextTree *> m_trees;
        std::vector<IrParserStructure *> m_components;

    protected:
        const LanguageRules *m_rules;

        // Visibility
        IrVisibility m_defaultVisibility;
        IrVisibility m_visibility;

        // Compilation flags
        bool m_definitionsResolved;
        bool m_expanded;
        bool m_validated;
        bool m_typesChecked;
        bool m_instantiationChecked;

        bool m_checkReferences;

    public:
        // Debugging
        void writeReferencesToFile(std::ofstream &file, IrContextTree *context, int tabLevel = 0);

        // Building
    public:
        NodeOutput *generateNodeOutput(IrContextTree *context, NodeProgram *program, NodeContainer *container);
        Node *generateNode(IrContextTree *context, NodeProgram *program, NodeContainer *container);

        virtual NodeOutput *_generateNodeOutput(IrContextTree *context, NodeProgram *program, NodeContainer *container);
        virtual Node *_generateNode(IrContextTree *context, NodeProgram *program, NodeContainer *container);
    };

} /* namespace piranha */

#endif /* PIRANHA_IR_PARSER_STRUCTURE_H */
