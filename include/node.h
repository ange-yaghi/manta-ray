#ifndef NODE_H
#define NODE_H

#include <node_type.h>
#include <node_output.h>

#include <string>
#include <vector>

namespace manta {

	struct IntersectionPoint;
	class StackAllocator;

	class Node {
	public:
		struct NodeSessionMemory {
			unsigned char memory[64];
			void *extraMemory;
		};

		struct NodeInputPort {
			pNodeInput *input;
			const char *name;
		};

		struct NodeOutputPort {
			NodeOutput *output;
			const char *name;
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

		void connectInput(pNodeInput input, const char *name);
		int getInputCount() const { return (int)m_inputs.size(); }

		NodeOutput *getOutput(const char *name) const;
		int getOutputCount() const { return (int)m_outputs.size(); }

		bool isInitialized() const { return m_initialized; }
		bool isEvaluated() const { return m_evaluated; }

		// Main Interface
	public:
		virtual void initializeSessionMemory(const IntersectionPoint *surfaceInteraction, NodeSessionMemory *memory, StackAllocator *stackAllocator) const;

	protected:
		virtual void _initialize();
		virtual void _evaluate();
		virtual void _destroy();

		virtual void registerOutputs();
		virtual void registerInputs();

	protected:
		int m_id;
		std::string m_name;

	protected:
		std::vector<NodeInputPort> m_inputs;

		void registerInput(pNodeInput *node, const char *name) { m_inputs.push_back({node, name}); }

		std::vector<NodeOutputPort> m_outputs;
		void registerOutput(NodeOutput *node, const char *name);

	private:
		// State variables

		// Initialization status
		bool m_initialized;
		bool m_evaluated;
	};

} /* namespace manta */

#endif /* NODE_H */
