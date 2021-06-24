#ifndef MANTARAY_COMPLEX_MAP_2D_NODE_OUTPUT_H
#define MANTARAY_COMPLEX_MAP_2D_NODE_OUTPUT_H

#include <piranha.h>

#include "manta_math.h"
#include "complex_map_interface_node.h"
#include "object_reference_node_output.h"

namespace manta {

	class ComplexMap2dNodeOutput : public ObjectReferenceNodeOutput<ComplexMap2D> {
	public:
		ComplexMap2dNodeOutput() {
			/* void */
		}

		virtual ~ComplexMap2dNodeOutput() {
			/* void */
		}

	protected:
		virtual piranha::Node *newInterface(piranha::NodeAllocator *nodeAllocator) {
			ComplexMapInterfaceNode *complexMapInterface =
				nodeAllocator->allocate<ComplexMapInterfaceNode>();
			complexMapInterface->initialize();
			complexMapInterface->connectInput(this, "__in", nullptr);

			return complexMapInterface;
		}
	};

} /* namespace manta */

#endif /* MANTARAY_COMPLEX_MAP_2D_NODE_OUTPUT_H */
