#ifndef PIRANHA_NODE_H
#define PIRANHA_NODE_H

#include "channel_type.h"
#include "node_output.h"

#include <string>
#include <vector>

namespace piranha {

    struct IntersectionPoint;
    class StackAllocator;
    class IrContextTree;
    class IrParserStructure;
    class IrAttributeDefinition;
    class NodeProgram;
    class NodeContainer;
    class Assembly;

    class Node {
    public:
        struct NodeInputPort {
            pNodeInput *input;
            Node *nodeInput;
            Node *dependency;
            std::string name;
            bool modifiesInput;
            bool enableInput;
        };

        struct NodeOutputPort {
            NodeOutput *output;
            std::string name;
        };

        struct NodeOutputPortReference {
            NodeOutput *const *output;
            Node *nodeOutput;
            std::string name;
        };

        struct PortSkeleton {
            NodeOutput *output;
            Node *nodeOutput;
            std::string name;

            IrAttributeDefinition *definition;
            IrContextTree *context;
            NodeContainer *container;
        };

        struct PortInfo {
            bool modifiesInput;
            bool isToggle;
            bool isAlias;
        };

    public:
        Node();
        virtual ~Node();

        Node *getAliasNode();
        NodeOutput *getAliasOutput();

        void initialize();
        bool evaluate();
        void destroy();

        void throwError(const std::string errorMessage);

        void setId(int id) { m_id = id; }
        int getId() const { return m_id; }

        void setName(const std::string &name) { m_name = name; }
        std::string getName() const { return m_name; }

        void setBuiltinName(const std::string &name) { m_builtinName = name; }
        std::string getBuiltinName() const { return m_builtinName; }

        void connectEnableInput(pNodeInput input, Node *dependency);
        void connectInput(pNodeInput input, const std::string &name, Node *dependency, Node *nodeInput=nullptr);
        void connectInput(pNodeInput input, int index, Node *dependency, Node *nodeInput=nullptr);
        bool getInputPortInfo(const std::string &name, PortInfo *info) const;
        const NodeInputPort *getInput(int index) const { return &m_inputs[index]; }
        int getInputCount() const { return (int)m_inputs.size(); }

        void addPortSkeleton(const PortSkeleton &skeleton);
        Node *generateNodeOutput(const std::string &name);
        NodeOutput *generateOutput(const std::string &name);

        Node *generateAliasNode();
        NodeOutput *generateAliasOutput();

        NodeOutput *getPrimaryOutput() const;
        Node *getPrimaryNode() const;
        NodeOutput *getOutput(const std::string &name) const;
        Node *getNodeOutput(const std::string &name) const;
        int getOutputCount() const { return (int)m_outputs.size(); }
        const NodeOutputPort *getOutput(int index) const { return &m_outputs[index]; }
        bool getOutputPortInfo(const std::string &name, PortInfo *info) const;

        int getOutputReferenceCount() const { return (int)m_outputReferences.size(); }
        const NodeOutputPortReference *getOutputReference(int index) const { return &m_outputReferences[index]; }

        bool isInitialized() const { return m_initialized; }
        bool isEvaluated() const { return m_evaluated; }

        void setIrStructure(IrParserStructure *irStructure) { m_irStructure = irStructure; }
        IrParserStructure *getIrStructure() const { return m_irStructure; }

        void setIrContext(IrContextTree *context) { m_context = context; }
        IrContextTree *getContext() const { return m_context; }

        void setProgram(NodeProgram *program) { m_program = program; }
        NodeProgram *getProgram() const { return m_program; }

        void setContainer(NodeContainer *container) { m_container = container; }
        NodeContainer *getContainer() const { return m_container; }

        bool checkEnabled();
        bool isEnabled() const { return m_enabled; }

        void addDependency(Node *node) { m_dependencyChain.push_back(node); }

        NodeOutput *getInterfaceInput() const;
        void setInterfaceInput(pNodeInput *output) { m_interfaceInput = output; }

        virtual void writeAssembly(std::fstream &file, Assembly *assembly, int indent) const;

        void setPrimaryOutput(const std::string &name);

        void setVirtual(bool isVirtual) { m_isVirtual = isVirtual; }
        bool isVirtual() const { return m_isVirtual; }

        virtual bool isLiteral() const { return false; }

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual void registerOutputs();
        virtual void registerInputs();

    protected:
        int m_id;
        std::string m_name;
        std::string m_builtinName;

        IrContextTree *m_context;
        IrParserStructure *m_irStructure;

        bool m_isVirtual;

    protected:
        std::vector<NodeInputPort> m_inputs;
        void registerInput(
            pNodeInput *node, const std::string &name, bool modifiesInput=false, bool enableInput=false);

        std::vector<NodeOutputPort> m_outputs;
        void registerOutput(NodeOutput *node, const std::string &name);

        std::vector<NodeOutputPortReference> m_outputReferences;
        void registerOutputReference(NodeOutput *const *output, const std::string &name, Node *node = nullptr);

        PortSkeleton *getSkeleton(const std::string &name);
        std::vector<PortSkeleton> m_portSkeletons;

    protected:
        pNodeInput *m_interfaceInput;
        std::string m_primaryOutput;

        pNodeInput *m_enableInput;
        Node *m_enableInputDependency;

    private:
        // State variables

        // Initialization status
        bool m_initialized;
        bool m_evaluated;
        bool m_checkedEnabled;

        bool m_enabled;

    protected:
        // Context
        NodeProgram *m_program;
        NodeContainer *m_container;

        std::vector<Node *> m_dependencyChain;

    protected:
        // Run-time
        bool m_runtimeError;
    };

} /* namespace piranha */

#endif /* PIRANHA_NODE_H */
