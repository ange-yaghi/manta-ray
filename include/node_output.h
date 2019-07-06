#ifndef NODE_OUTPUT_H
#define NODE_OUTPUT_H

#include <node_type.h>
#include <manta_math.h>

#include <string>
#include <vector>

namespace manta {

	struct IntersectionPoint;
	class Node;

	class NodeOutput {
	public:
		static const int MAX_DIMENSIONS = 4;

	public:
		NodeOutput(const NodeType *singleType);
		virtual ~NodeOutput();

		const NodeType *getType() const { return m_singleType; }

		void initialize();

		virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const { /* void */ }
		virtual void discreteSample2D(int x, int y, void *target) const { /* void */ }
		virtual void fullCompute(void *target) const { /* void */ }
		virtual void getDataReference(const void **target) const { *target = nullptr; }

		void evaluate();
		virtual void registerInputs() {}

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

		Node *getInterface() const { return m_interface; }

	protected:
		void setDimensionSize(int dim, int size) { m_dimensions[dim] = size; }
		void setDimensions(int dimensionCount) { m_dimensionCount = dimensionCount; }

		virtual void _evaluateDimensions() { setDimensions(1); setDimensionSize(0, 1); };

		void overrideType(const NodeType *type) { m_singleType = type; }

		virtual Node *generateInterface() { return nullptr; }

	private:
		int m_dimensions[MAX_DIMENSIONS];
		int m_dimensionCount;
		const NodeType *m_singleType;

		std::string m_name;

		Node *m_parentNode;
		Node *m_interface;

		bool m_dimensionsEvaluated;
		bool m_evaluated;

	protected:
		virtual void registerInput(NodeOutput **nodeInput) { m_inputs.push_back(nodeInput); }
		int getInputCount() const { return (int)m_inputs.size(); }

		std::vector<NodeOutput **> m_inputs;
	};

	// Type to reduce confusion
	typedef NodeOutput * pNodeInput;

} /* namespace manta */

#endif /* NODE_OUTPUT_H */
