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
		int faceHint;
		int subdivisionHint;
		int globalHint;

		math::real su, sv, sw;
		
		bool valid;
	};

}

#endif /* COARSE_INTERSECTION_H */
