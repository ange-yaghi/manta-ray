#ifndef MANTARAY_CAMERA_RAY_EMITTER_GROUP_H
#define MANTARAY_CAMERA_RAY_EMITTER_GROUP_H

#include "object_reference_node.h"

#include "manta_math.h"
#include "stack_allocator.h"

namespace manta {

    class ImagePlane;
    class Sampler;
    class CameraRayEmitter;
    class RayContainer;

    class CameraRayEmitterGroup : public ObjectReferenceNode<CameraRayEmitterGroup> {
    public:
        CameraRayEmitterGroup();
        virtual ~CameraRayEmitterGroup();

        void setUp(const math::Vector &up) { m_up = up; }
        math::Vector getUp() const { return m_up; }

        ImagePlane *getImagePlane() const { return m_imagePlane; }
        void setImagePlane(ImagePlane *plane) { m_imagePlane = plane; }

        void setResolutionX(int resolution) { m_resolutionX = resolution; }
        int getResolutionX() const { return m_resolutionX; }

        void setResolutionY(int resolution) { m_resolutionY = resolution; }
        int getResolutionY() const { return m_resolutionY; }

        void setPlaneHeight(float planeHeight) { m_planeHeight = planeHeight; }
        math::real_d getPlaneHeight() const { return m_planeHeight; }

        void setPlaneDistance(float planeDistance) { m_planeDistance = planeDistance; }
        math::real_d getPlaneDistance() const { return m_planeDistance; }

        void setPosition(const math::Vector &position) { m_position = position; }
        math::Vector getPosition() const { return m_position; }

        void setDirection(const math::Vector &direction) { m_direction = direction; }
        math::Vector getDirection() const { return m_direction; }

        void setSampler(Sampler *sampler) { m_sampler = sampler; }
        Sampler *getSampler() const { return m_sampler; }

        virtual void configure() = 0;

        virtual CameraRayEmitter *createEmitter(int ix, int iy, 
            StackAllocator *stackAllocator) const = 0;
        void freeEmitter(CameraRayEmitter *rayEmitter, StackAllocator *stackAllocator) const;

    protected:
        template<typename t_RayEmitterType>
        t_RayEmitterType *allocateEmitter(StackAllocator *stackAllocator) const {
            t_RayEmitterType *newEmitter = nullptr;
            if (stackAllocator == nullptr) {
                void *buffer = _aligned_malloc(sizeof(t_RayEmitterType), 16);
                newEmitter = new(buffer) t_RayEmitterType;
            }
            else {
                void *buffer = stackAllocator->allocate(sizeof(t_RayEmitterType), 16);
                newEmitter = new(buffer) t_RayEmitterType;
            }

            return newEmitter;
        }

    protected:
        virtual void _evaluate();
        virtual void _initialize();
        virtual void registerInputs();

        piranha::pNodeInput m_upInput;
        piranha::pNodeInput m_positionInput;
        piranha::pNodeInput m_directionInput;
        piranha::pNodeInput m_planeHeightInput;
        piranha::pNodeInput m_planeDistanceInput;
        piranha::pNodeInput m_imagePlaneInput;

    protected:
        math::Vector m_up;
        math::Vector m_position;
        math::Vector m_direction;

        int m_resolutionX;
        int m_resolutionY;

        math::real_d m_planeHeight;
        math::real_d m_planeDistance;

        Sampler *m_sampler;
        ImagePlane *m_imagePlane;
    };

} /* namespace manta */

#endif /* MANTARAY_CAMERA_RAY_EMITTER_GROUP_H */
