#include "light.h"

#include "cacheable_input.h"

namespace manta {

    class AreaLight : public Light {
    public:
        AreaLight();
        ~AreaLight();

        virtual math::Vector sampleIncoming(const IntersectionPoint &ref, const math::Vector2 &u, math::Vector *wi, math::real *pdf, math::real *depth) const;
        virtual math::real pdfIncoming(const IntersectionPoint &ref, const math::Vector &wi) const;

        virtual math::Vector L(const IntersectionPoint &ref, const math::Vector &wi) const;

        virtual bool intersect(const math::Vector &src, const math::Vector &dir, math::real *depth) const;

        math::real getArea() const;

        void setUp(const math::Vector &up) { m_up = up; }
        void setDirection(const math::Vector &direction) { m_direction = direction; }
        void setOrigin(const math::Vector &origin) { m_origin = origin; }
        void setIntensity(const math::Vector &intensity) { m_intensity = intensity; }

        void setWidth(math::real width) { m_width = width; }
        void setHeight(math::real height) { m_height = height; }

    protected:
        virtual void _evaluate();
        virtual void registerInputs();

        piranha::pNodeInput m_upInput;
        piranha::pNodeInput m_directionInput;
        piranha::pNodeInput m_originInput;
        piranha::pNodeInput m_intensityInput;
        piranha::pNodeInput m_widthInput;
        piranha::pNodeInput m_heightInput;

    protected:
        math::real pdfIncoming(const math::Vector &ref, const math::Vector &p, const math::Vector &wi) const;

        math::Vector m_up;
        math::Vector m_direction;
        math::Vector m_origin;
        math::Vector m_intensity;

        math::real m_width;
        math::real m_height;
    };

} /* namespace manta */
