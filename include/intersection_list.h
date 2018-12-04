#ifndef INTERSECTION_LIST_H
#define INTERSECTION_LIST_H

#include <vector>

namespace manta {

	// Forward declarations
	class StackAllocator;
	class SceneObject;
	class SceneGeometry;

	struct CoarseIntersection {
		SceneObject *sceneObject;
		const SceneGeometry *sceneGeometry;
		double depth;
		int locationHint;
	};

	class IntersectionList {
	public:
		IntersectionList();
		~IntersectionList();

		CoarseIntersection *newIntersection();
		void destroy();

		void setStack(StackAllocator *stack) { m_stack = stack; }
		StackAllocator *getStack() const { return m_stack; }
		bool isUsingStack() const { return m_stack != nullptr; }

		CoarseIntersection *getIntersection(int i);
		int getIntersectionCount() const { return m_intersectionCount; }

	protected:
		StackAllocator *m_stack;

		union {
			CoarseIntersection *m_list;

			// If not using the stack, use a vector
			std::vector<CoarseIntersection *> *m_intersectionVector;
		};

		int m_intersectionCount;
	};

}

#endif /* INTERSECTION_LIST_H */
