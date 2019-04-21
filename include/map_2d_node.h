#ifndef MAP_2D_NODE_H
#define MAP_2D_NODE_H

#include <node.h>

#include <manta_math.h>
#include <vector_map_2d.h>
#include <complex_map_2d.h>
#include <scalar_map_2d.h>

namespace manta {

	template <typename MapType>
	class Map2DNode : public Node {
	public:
		Map2DNode() {
			m_map = nullptr;
		}

		virtual ~Map2DNode() {
			/* void */
		}

		virtual void _initialize() { }
		virtual void _destroy() { }

		const MapType *getMap() const { return m_map; }
		void setMap(const MapType *map) { m_map = map; }

	protected:
		const MapType *m_map;
	};

	typedef Map2DNode<VectorMap2D> VectorMap2DNode;
	typedef Map2DNode<ComplexMap2D> ComplexMap2DNode;
	typedef Map2DNode<RealMap2D> RealMap2DNode;
	typedef Map2DNode<RealMap2D_d> DoubleMap2DNode;
	typedef Map2DNode<RealMap2D_f> FloatMap2DNode;

} /* namespace manta */

#endif /* MAP_2D_NODE_H */
