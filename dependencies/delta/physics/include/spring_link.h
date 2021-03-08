#ifndef DELTA_BASIC_SPRING_LINK_H
#define DELTA_BASIC_SPRING_LINK_H

#include "rigid_body_link.h"

namespace dphysics {

    class SpringLink : public RigidBodyLink {
    public:
        enum FALLOFF_PATTERN {
            FALLOFF_LINEAR,
            FALLOFF_R2,

            FALLOFF_UNDEFINED
        };

    public:
        SpringLink();
        ~SpringLink();

        virtual int GenerateCollisions(Collision *collisionArray);

        void SetConnectionPoints(ysVector &p1, ysVector &p2) {
            m_relativePos1 = p1;
            m_relativePos2 = p2;
        }

        ysVector GetConnection1() const { return m_relativePos1; }
        ysVector GetConnection2() const { return m_relativePos2; }

        void SetLength(float length) { m_length = length; }
        float GetLength() const { return m_length; }

        void SetFalloffPattern(FALLOFF_PATTERN pattern) { m_falloffPattern = pattern; }
        FALLOFF_PATTERN GetFalloffPattern() const { return m_falloffPattern; }

    protected:
        ysVector m_relativePos1;
        ysVector m_relativePos2;

        float m_length;
        float m_strength;
        FALLOFF_PATTERN m_falloffPattern;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_SPRING_LINK_H */
