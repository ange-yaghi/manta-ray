#ifndef MANTARAY_CONSTRUCTED_STRING_NODE_OUTPUT_H
#define MANTARAY_CONSTRUCTED_STRING_NODE_OUTPUT_H

#include <piranha.h>

#include "string_node_output.h"
#include "intersection_point.h"

namespace manta {

	class ConstructedStringNodeOutput : public StringNodeOutput {
	public:
		ConstructedStringNodeOutput();
		virtual ~ConstructedStringNodeOutput();

		virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
		virtual void discreteSample2D(int x, int y, void *target) const;
		virtual void fullCompute(void *target) const;

		piranha::pNodeInput *getStringConnection() { return &m_stringInput; }
		const NodeOutput *getStringOutput() { return m_stringInput; }

		virtual void registerInputs();

	protected:
		piranha::pNodeInput m_stringInput;
	};

} /* namespace manta */

#endif /* MANTARAY_CONSTRUCTED_VECTOR_NODE_OUTPUT_H */
