#ifndef KD_TREE_H
#define KD_TREE_H

#include <manta_math.h>
#include <geometry.h>
#include <scene_geometry.h>
#include <stack_list.h>
#include <primitives.h>
#include <runtime_statistics.h>

#include <vector>
#include <fstream>

#define OPTIMIZED_KD_TREE_NODE (true)

namespace manta {

	class Mesh;
	class SceneObject;
	class StackAllocator;

#if OPTIMIZED_KD_TREE_NODE
	struct KDTreeNode {
		union {
			math::real split;
			int singleObject;
			int objectOffset;
		};
		union {
			int flags;
			int primitiveCount;
			int aboveChild;
		};

		void initLeaf(int primitiveCount, int object) {
			flags = 0x3;
			this->primitiveCount |= (primitiveCount << 2);
			objectOffset = object;
		}

		void initInterior(int axis, int aboveChild, math::real s) {
			split = s;
			flags = axis;
			this->aboveChild |= (aboveChild << 2);
		}

		inline math::real getSplit() const { return split; }
		inline int getPrimitiveCount() const { return primitiveCount >> 2; }
		inline int getObjectOffset() const { return objectOffset; }
		inline int getSplitAxis() const { return (flags & 0x3); }
		inline bool isLeaf() const { return (flags & 0x3) == 0x3; }
		inline int getAboveChild() const { return aboveChild >> 2; }
	};
#else
	struct KDTreeNode {
		math::real split;
		int singleObject;
		int objectOffset;

		int flags;
		int primitiveCount;
		int aboveChild;

		void initLeaf(int primitiveCount, int object) {
			flags = 0x3;
			this->primitiveCount = primitiveCount;
			objectOffset = object;
		}

		void initInterior(int axis, int aboveChild, math::real s) {
			split = s;
			flags = axis;
			this->aboveChild = aboveChild;
		}

		inline math::real getSplit() const { return split; }
		inline int getPrimitiveCount() const { return primitiveCount; }
		inline int getObjectOffset() const { return objectOffset; }
		inline int getSplitAxis() const { return flags; }
		inline bool isLeaf() const { return flags == 0x3; }
		inline int getAboveChild() const { return aboveChild; }
	};
#endif /* OPTIMIZED_KD_TREE_NODE */

	struct KDBoundingVolume {
		AABB bounds;
		int faceListOffset;
		int padding[3];
	};

	struct KDBoundEdge {
		enum EdgeType {
			Start,
			End
		};

		KDBoundEdge() {

		}

		KDBoundEdge(math::real t, int primitiveIndex, bool start) : t(t), primitiveIndex(primitiveIndex) {
			edgeType = start ? EdgeType::Start : EdgeType::End;
		}

		math::real t;
		int primitiveIndex;
		EdgeType edgeType;
	};

	struct KDTreeWorkspace {
		// Accumulators
		std::vector<int> faces;
		
		// Constants
		Mesh *mesh;
		KDBoundEdge *edges[3];
		AABB *allFaceBounds;
		int maxPrimitives;
	};

	struct KDJob {
		const KDTreeNode *node;
		math::real tmin, tmax;
	};

	class KDTree : public SceneGeometry {
	public:
		KDTree();
		~KDTree();

		void initialize(math::real width, const math::Vector &position);
		void destroy();

		virtual bool findClosestIntersection(const LightRay *ray, CoarseIntersection *intersection, math::real minDepth, math::real maxDepth, StackAllocator *s /**/ STATISTICS_PROTOTYPE) const;
		virtual math::Vector getClosestPoint(const CoarseIntersection *hint, const math::Vector &p) const;
		virtual void fineIntersection(const math::Vector &r, IntersectionPoint *p, const CoarseIntersection *hint) const;
		virtual bool fastIntersection(const LightRay *ray) const;

		void analyze(Mesh *mesh, int maxSize);

		int createNode();
		int createNodeVolume();
		KDTreeNode *getNode(int index) { return &m_nodes[index]; }

		void initLeaf(int node, const std::vector<int> &faces, const AABB &bounds, KDTreeWorkspace *workspace);

		void writeToObjFile(const char *fname) const;

	protected:
		void analyze(int currentNode, AABB *nodeBounds, const std::vector<int> &faces, int badRefines, int depth, KDTreeWorkspace *workspace);

		Mesh *m_mesh;

		AABB m_bounds;
		KDTreeNode *m_nodes;
		int m_nodeCount;
		int m_nodeCapacity;

		KDBoundingVolume *m_nodeVolumes;
		int m_volumeCount;
		int m_volumeCapacity;

		int *m_faceLists;
		int m_faceCount;

		std::vector<AABB> m_nodeBounds;

		math::real m_width;
	};

} /* namespace manta */

#endif /* KD_TREE_H */
