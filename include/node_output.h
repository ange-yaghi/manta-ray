#ifndef NODE_OUTPUT_H
#define NODE_OUTPUT_H

#include <node.h>
#include <node_type.h>

#include <manta_math.h>

namespace manta {

	struct IntersectionPoint;

	class NodeOutput {
	public:
		static const int MAX_DIMENSIONS = 4;

	public:
		NodeOutput(const char *type) { m_type.initialize(type); }
		virtual ~NodeOutput() {}

		virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const {};
		virtual void discreteSample2D(int x, int y, void *target) const {};
		virtual void fullOutput(void **target) const {};

		int getSize(int dim) const { return m_dimensions[dim]; }
		int getDimensions() const { return m_dimensionCount; }

	protected:
		int setDimension(int size, int dim) { m_dimensions[dim] = size; }

	private:
		int m_dimensions[MAX_DIMENSIONS];
		int m_dimensionCount;
		NodeType m_type;
	};

} /* namespace manta */

#endif /* NODE_OUTPUT_H */
