#ifndef MANTARAY_COARSE_INTERSECTION_H
#define MANTARAY_COARSE_INTERSECTION_H

#include "manta_math.h"

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

#endif /* MANTARAY_COARSE_INTERSECTION_H */
