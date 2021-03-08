#ifndef DELTA_BASIC_EXPANDING_SPRING_H
#define DELTA_BASIC_EXPANDING_SPRING_H

#include "mass_spring_system.h"

namespace dphysics {

    class MSSExpandingSpring : public MSSSpring {
    public:
        MSSExpandingSpring();
        ~MSSExpandingSpring();

        virtual void Update(float timeDelta);
        virtual float GetLength(float timeDelta = 0.0f) const;

        void SetExpansionRate(float expansionRate) { m_expansionRate = expansionRate; }

    protected:
        float m_expansionRate;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_EXPANDING_SPRING_H */
