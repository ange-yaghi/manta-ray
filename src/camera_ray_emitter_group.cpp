#include <camera_ray_emitter_group.h>

manta::CameraRayEmitterGroup::CameraRayEmitterGroup() {
	/* void */
}

manta::CameraRayEmitterGroup::~CameraRayEmitterGroup() {
	/* void */
}

void manta::CameraRayEmitterGroup::freeEmitter(CameraRayEmitter *rayEmitter, StackAllocator *stackAllocator) const {
	stackAllocator->free(rayEmitter);
}
