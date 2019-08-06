#ifndef MANTARAY_LENS_H
#define MANTARAY_LENS_H

#include "object_reference_node.h"

#include "manta_math.h"

namespace manta {

    // Forward declarations
    class Aperture;
    class LightRay;

    class Lens : public ObjectReferenceNode<Lens> {
    public:
        struct LensScanHint {
            math::real centerX;
            math::real centerY;
            math::real radius;
            bool failed;
        };
    public:
        Lens();
        virtual ~Lens();

        void setPosition(const math::Vector &position) { m_position = position; }
        math::Vector getPosition() const { return m_position; }

        void setDirection(const math::Vector &direction) { m_direction = direction; }
        math::Vector getDirection() const { return m_direction; }

        void setRadius(math::real radius) { m_radius = radius; }
        math::real getRadius() const { return m_radius; }

        void setUp(const math::Vector &up) { m_up = up; }
        math::Vector getUp() const { return m_up; }

        virtual void setFocus(math::real dist) = 0;
        virtual void setFocalLength(math::real focalLength) = 0;
        virtual bool transformRay(const LightRay *inputRay, LightRay *outputRay) const = 0;

        math::Vector getSensorLocation() const { return m_sensorLocation; }
        math::Vector getSensorUp() const { return m_up; }
        math::Vector getSensorRight() const;

        virtual void configure() = 0;
        virtual void update() = 0;

        void setAperture(Aperture *aperture) { m_aperture = aperture; }
        Aperture *getAperture() const { return m_aperture; }

        void setSensorResolutionX(int w) { m_sensorResolutionX = w; }
        int getSensorResolutionX() const { return m_sensorResolutionX; }

        void setSensorResolutionY(int h) { m_sensorResolutionY = h; }
        int getSensorResolutionY() const { return m_sensorResolutionY; }

        virtual math::Vector getSensorElement(int x, int y) const = 0;
        virtual void lensScan(const math::Vector &sensorElement,
            int div, math::real span, LensScanHint *target) const = 0;
        virtual bool generateOutgoingRay(const math::Vector &sensorElement,
            const LensScanHint *hint, LightRay *targetRay) const = 0;

        void setSensorWidth(math::real width) { m_sensorWidth = width; }
        math::real getSensorWidth() const { return m_sensorWidth; }

        void setSensorHeight(math::real height) { m_sensorHeight = height; }
        math::real getSensorHeight() const { return m_sensorHeight; }

    protected:
        math::Vector m_position;
        math::Vector m_direction;
        math::Vector m_up;
        math::Vector m_sensorLocation;
        math::real m_radius;

        math::real m_sensorWidth;
        math::real m_sensorHeight;

        int m_sensorResolutionX;
        int m_sensorResolutionY;

        Aperture *m_aperture;

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual void registerInputs();
        virtual void registerOutputs();

        piranha::pNodeInput m_positionInput;
        piranha::pNodeInput m_directionInput;
        piranha::pNodeInput m_upInput;
        piranha::pNodeInput m_radiusInput;
        piranha::pNodeInput m_sensorWidthInput;
        piranha::pNodeInput m_sensorHeightInput;
        piranha::pNodeInput m_resolutionXInput;
        piranha::pNodeInput m_resolutionYInput;
        piranha::pNodeInput m_apertureInput;
    };

} /* namespace manta */

#endif /* MANTARAY_LENS_H */
