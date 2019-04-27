#ifndef NODE_OUTPUT_H
#define NODE_OUTPUT_H

#include <node.h>
#include <node_type.h>

#include <manta_math.h>

namespace manta {

	struct IntersectionPoint;
	class Node;

	class NodeOutput {
	public:
		static const int MAX_DIMENSIONS = 4;

	public:
		NodeOutput(const NodeType *singleType) { m_singleType = singleType; }
		virtual ~NodeOutput() {}

		virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const {};
		virtual void discreteSample2D(int x, int y, void *target) const {};
		virtual void fullOutput(const void **target) const {};

		int getSize(int dim) const { return m_dimensions[dim]; }
		int getDimensions() const { return m_dimensionCount; }

		void setName(const std::string &name) { m_name = name; }
		const std::string &getName() const { return m_name; }

		void setParentNode(Node *parentNode) { m_parentNode = parentNode; }
		Node *getParentNode() const { return m_parentNode; }

	protected:
		void setDimensionSize(int dim, int size) { m_dimensions[dim] = size; }
		void setDimensions(int dimensionCount) { m_dimensionCount = dimensionCount; }

	private:
		int m_dimensions[MAX_DIMENSIONS];
		int m_dimensionCount;
		const NodeType *m_singleType;

		std::string m_name;

		Node *m_parentNode;
	};

} /* namespace manta */

#endif /* NODE_OUTPUT_H */
