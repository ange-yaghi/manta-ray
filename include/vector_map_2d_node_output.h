#ifndef VECTOR_MAP_2D_NODE_OUTPUT_H
#define VECTOR_MAP_2D_NODE_OUTPUT_H

#include <vector_node_output.h>

#include <node_type.h>
#include <manta_math.h>
#include <vector_map_2d.h>

namespace manta {

	class VectorMap2DNodeOutput : public VectorNodeOutput {
	public:
		VectorMap2DNodeOutput() {
			m_map = nullptr;

			setDimensions(2);
		}

		virtual ~VectorMap2DNodeOutput() {
			/* void */
		}

		virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
		virtual void discreteSample2D(int x, int y, void *target) const;
		virtual void fullOutput(const void **target) const;

		const VectorMap2D *getMap() const { return m_map; }
		void setMap(const VectorMap2D *map);

	protected:
		const VectorMap2D *m_map;
	};

} /* namespace manta */

#endif /* VECTOR_MAP_2D_NODE_OUTPUT_H */
