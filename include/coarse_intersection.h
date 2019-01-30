#ifndef COARSE_INTERSECTION_H
#define COARSE_INTERSECTION_H

#include <manta_math.h>

namespace manta {

	class SceneObject;
	class SceneGeometry;

	struct CoarseIntersection {
		SceneObject *sceneObject;
		const SceneGeometry *sceneGeometry;
		math::real depth;
		int locationHint;
		int globalHint;
		bool valid;
	};

}

#endif /* COARSE_INTERSECTION_H */
