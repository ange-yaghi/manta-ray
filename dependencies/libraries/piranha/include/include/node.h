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
    class NodeProgram;

    class Node {
    public:
        struct NodeSessionMemory {
            unsigned char memory[64];
            void *extraMemory;
        };

        struct NodeInputPort {
            pNodeInput *input;
            std::string name;
            bool modifiesInput;
        };

        struct NodeOutputPort {
            NodeOutput *output;
            std::string name;
        };

        struct NodeOutputPortReference {
            NodeOutput *const *output;
            std::string name;
        };

        struct PortInfo {
            bool modifiesInput;
        };

    public:
        Node();
        virtual ~Node();

        void initialize();
        void evaluate();
        void destroy();

        void setId(int id) { m_id = id; }
        int getId() const { return m_id; }

        void setName(const std::string &name) { m_name = name; }
        std::string getName() const { return m_name; }

        void setBuiltinName(const std::string &name) { m_builtinName = name; }
        std::string getBuiltinName() const { return m_builtinName; }

        void connectInput(pNodeInput input, const std::string &name);
        void connectInput(pNodeInput input, int index);
        void connectDefaultInput(pNodeInput input);
        bool getInputPortInfo(const std::string &name, PortInfo *info) const;
        const NodeInputPort *getInput(int index) const { return &m_inputs[index]; }
        int getInputCount() const { return (int)m_inputs.size(); }

        NodeOutput *getPrimaryOutput() const;
        NodeOutput *getOutput(const std::string &name) const;
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

    protected:
        std::vector<NodeInputPort> m_inputs;
        void registerInput(
			pNodeInput *node, const std::string &name, bool modifiesInput=false);

        std::vector<NodeOutputPort> m_outputs;
        void registerOutput(NodeOutput *node, const std::string &name);

        std::vector<NodeOutputPortReference> m_outputReferences;
        void registerOutputReference(NodeOutput *const *node, const std::string &name);

        void setPrimaryOutput(NodeOutput *node);
        void setPrimaryOutputReference(NodeOutput **node);

    protected:
        NodeOutput *m_primaryOutput;
        NodeOutput **m_primaryReference;

    private:
        // State variables

        // Initialization status
        bool m_initialized;
        bool m_evaluated;

    protected:
        // Context
        NodeProgram *m_program;
    };

} /* namespace piranha */

#endif /* PIRANHA_NODE_H */
