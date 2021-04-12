#include "light.h"

namespace manta {

    class AreaLight : public Light {
    public:
        AreaLight();
        ~AreaLight();

        virtual math::Vector sampleIncoming(const IntersectionPoint &ref, const math::Vector2 &u, math::Vector *wi, math::real *pdf) const;
        virtual math::real pdfIncoming(const IntersectionPoint &ref, const math::Vector &wi) const;

        math::real getArea() const;

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
