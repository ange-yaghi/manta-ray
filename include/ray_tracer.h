#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <atomic>
#include <mutex>

namespace manta {

	class LightRay;
	class Scene;
	class RayEmitter;
	class RayEmitterGroup;
	class SceneObject;
	struct IntersectionPoint;

	class RayTracer {
	public:
		RayTracer();
		~RayTracer();

		void traceAll(const Scene *scene, RayEmitterGroup *rayEmitterGroup);

	protected:
		void depthCull(const Scene *scene, const LightRay *ray, SceneObject **closestObject, IntersectionPoint *point) const;

		void traceRay(const Scene *scene, LightRay *ray, int degree) const;
		void traceRayEmitterGroup(const Scene *scene, const RayEmitterGroup *rayEmitterGroup) const;
		void traceRayEmitter(const Scene *scene, const RayEmitter *emitter) const;
		//void traceRayEmitterThread(const Scene *scene, const RayEmitter *emitter, int start, int end);
		void traceRayEmitterGroupThread(const Scene *scene, const RayEmitterGroup *rayEmitterGroup, int start, int end);

		std::atomic<int> m_currentRay;
		std::mutex m_outputLock;
		int m_rayCount;
	};

} /* namespace manta */

#endif /* RAY_TRACER_H */