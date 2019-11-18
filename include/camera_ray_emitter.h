#ifndef MANTARAY_CAMERA_RAY_EMITTER_H
#define MANTARAY_CAMERA_RAY_EMITTER_H

#include "manta_math.h"

namespace manta {

    class Sampler;
    class StackAllocator;
    class LightRay;

    class CameraRayEmitter {
    public:
        CameraRayEmitter();
        virtual ~CameraRayEmitter();

        virtual void initialize();
        virtual void generateRay(LightRay *ray) const = 0;

        void setSampler(Sampler *sampler) { m_sampler = sampler; }
        Sampler *getSampler() const { return m_sampler; }

        void setSampleCount(int samples) { m_sampleCount = samples; }
        int getSampleCount() const { return m_sampleCount; }

        math::Vector getPosition() const { return m_position; }
        void setPosition(const math::Vector &v) { m_position = v; }

        math::Vector getDirection() const { return m_direction; }
        void setDirection(const math::Vector &v) { m_direction = v; }

        math::Vector getUp() const { return m_up; }
        void setUp(const math::Vector &up) { m_up = up; }

        math::Vector getRight() const { return m_right; }
        void setRight(const math::Vector &right) { m_right = right; }

        math::Vector getImagePlaneOrigin() const { return m_imagePlaneOrigin; }
        void setImagePlaneOrigin(const math::Vector &imagePlaneOrigin) { m_imagePlaneOrigin = imagePlaneOrigin; }

        math::Vector2 getPixelIncrement() const { return m_pixelIncrement; }
        void setPixelIncrement(const math::Vector2 &increment) { m_pixelIncrement = increment; }

        void setImagePlaneCoordinates(int x, int y) { m_pixelX = x; m_pixelY = y; }
        int getPixelX() const { return m_pixelX; }
        int getPixelY() const { return m_pixelY; }

        void setStackAllocator(StackAllocator *s) { m_stackAllocator = s; }
        StackAllocator *getStackAllocator() { return m_stackAllocator; }

        math::Vector transformToImagePlane(const math::Vector2 coordinates) const;

    protected:
        Sampler *m_sampler;

        math::Vector m_position;
        math::Vector m_direction;
        math::Vector m_up;
        math::Vector m_right;
        math::Vector m_imagePlaneOrigin;
        math::Vector2 m_pixelIncrement;

        int m_pixelX;
        int m_pixelY;

        int m_sampleCount;

        StackAllocator *m_stackAllocator;
    };

} /* namespace manta */

#endif /* MANTARAY_CAMERA_RAY_EMITTER_H */
