#ifndef NODE_OUTPUT_H
#define NODE_OUTPUT_H

#include <node_type.h>
#include <manta_math.h>

#include <string>

namespace manta {

	struct IntersectionPoint;
	class Node;

	class NodeOutput {
	public:
		static const int MAX_DIMENSIONS = 4;

	public:
		NodeOutput(const NodeType *singleType) { m_singleType = singleType; m_dimensionsEvaluated = false; }
		virtual ~NodeOutput() { /* void */ }

		const NodeType *getType() const { return m_singleType; }

		virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const { /* void */ }
		virtual void discreteSample2D(int x, int y, void *target) const { /* void */ }
		virtual void fullCompute(void *target) const { /* void */ }
		virtual void getDataReference(const void **target) const { *target = nullptr; }

		int getSize(int dim) const { return m_dimensions[dim]; }
		int getDimensions() const { return m_dimensionCount; }

		void setName(const std::string &name) { m_name = name; }
		const std::string &getName() const { return m_name; }

		void setParentNode(Node *parentNode) { m_parentNode = parentNode; }
		Node *getParentNode() const { return m_parentNode; }

		void evaluateDimensions() {
			if (m_dimensionsEvaluated) return;
			_evaluateDimensions();
			m_dimensionsEvaluated = true;
		}

		bool areDimensionsEvaluated() const { return m_dimensionsEvaluated; }

	protected:
		void setDimensionSize(int dim, int size) { m_dimensions[dim] = size; }
		void setDimensions(int dimensionCount) { m_dimensionCount = dimensionCount; }

		virtual void _evaluateDimensions() { setDimensions(1); setDimensionSize(0, 1); };

		void overrideType(const NodeType *type) { m_singleType = type; }

	private:
		int m_dimensions[MAX_DIMENSIONS];
		int m_dimensionCount;
		const NodeType *m_singleType;

		std::string m_name;

		Node *m_parentNode;

		bool m_dimensionsEvaluated;
	};

	// Type to reduce confusion
	typedef const NodeOutput * pNodeInput;

} /* namespace manta */

#endif /* NODE_OUTPUT_H */
