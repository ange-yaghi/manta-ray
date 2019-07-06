#ifndef NODE_H
#define NODE_H

#include <node_type.h>
#include <node_output.h>

#include <string>
#include <vector>

namespace manta {

	struct IntersectionPoint;
	class StackAllocator;
	class SdlNode;

	class Node {
	public:
		struct NodeSessionMemory {
			unsigned char memory[64];
			void *extraMemory;
		};

		struct NodeInputPort {
			pNodeInput *input;
			std::string name;
		};

		struct NodeOutputPort {
			NodeOutput *output;
			std::string name;
		};

		struct NodeOutputPortReference {
			NodeOutput * const *output;
			std::string name;
		};

	public:
		Node();
		virtual ~Node();

		void destroySessionMemory(NodeSessionMemory *memory, StackAllocator *stackAllocator) const;

		void initialize();
		void evaluate();
		void destroy();

		void setId(int id) { m_id = id; }
		int getId() const { return m_id; }

		void setName(const std::string &name) { m_name = name; }
		std::string getName() const { return m_name; }

		void connectInput(pNodeInput input, const std::string &name);
		int getInputCount() const { return (int)m_inputs.size(); }

		NodeOutput *getPrimaryOutput() const;
		NodeOutput *getOutput(const std::string &name) const;
		int getOutputCount() const { return (int)m_outputs.size(); }

		int getOutputReferenceCount() const { return (int)m_outputReferences.size(); }

		bool isInitialized() const { return m_initialized; }
		bool isEvaluated() const { return m_evaluated; }

		void setSdlNode(SdlNode *node) { m_sdlNode = node; }
		SdlNode *getSdlNode() const { return m_sdlNode; }

		// Main Interface
	public:
		virtual void initializeSessionMemory(const IntersectionPoint *surfaceInteraction, 
			NodeSessionMemory *memory, StackAllocator *stackAllocator) const;

	protected:
		virtual void _initialize();
		virtual void _evaluate();
		virtual void _destroy();

		virtual void registerOutputs();
		virtual void registerInputs();

	protected:
		int m_id;
		std::string m_name;

		SdlNode *m_sdlNode;

	protected:
		std::vector<NodeInputPort> m_inputs;
		void registerInput(pNodeInput *node, const std::string &name);

		std::vector<NodeOutputPort> m_outputs;
		void registerOutput(NodeOutput *node, const std::string &name);

		std::vector<NodeOutputPortReference> m_outputReferences;
		void registerOutputReference(NodeOutput *const *node, const std::string &name);

		void setPrimaryOutput(NodeOutput *node);

	protected:
		NodeOutput *m_primaryOutput;

	private:
		// State variables

		// Initialization status
		bool m_initialized;
		bool m_evaluated;
	};

} /* namespace manta */

#endif /* NODE_H */
