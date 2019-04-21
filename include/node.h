#ifndef NODE_H
#define NODE_H

#include <node_type.h>

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

		struct NodeInput {
			Node **node;
			const char *name;
		};

	public:
		Node();
		virtual ~Node();

		virtual void initializeSessionMemory(const IntersectionPoint *surfaceInteraction, NodeSessionMemory *memory, StackAllocator *stackAllocator) const;
		void destroySessionMemory(NodeSessionMemory *memory, StackAllocator *stackAllocator) const;

		void initialize();
		void evaluate();
		void destroy();

		void setId(int id) { m_id = id; }
		int getId() const { return m_id; }

		void setName(const std::string &name) { m_name = name; }
		std::string getName() const { return m_name; }

		void connectInput(Node **node, const char *name);
		int getDependencyCount() const { return (int)m_dependencies.size(); }

		bool isInitialized() const { return m_initialized; }
		bool isEvaluated() const { return m_evaluated; }

	protected:
		virtual void _initialize();
		virtual void _evaluate();
		virtual void _destroy();

	protected:
		int m_id;
		std::string m_name;

	protected:
		std::vector<NodeInput> m_dependencies;
		virtual void registerDependencies();
		void registerDependency(Node **node, const char *name) { m_dependencies.push_back({node, name}); }

	private:
		// State variables

		// Initialization status
		bool m_initialized;
		bool m_evaluated;
	};

} /* namespace manta */

#endif /* NODE_H */
