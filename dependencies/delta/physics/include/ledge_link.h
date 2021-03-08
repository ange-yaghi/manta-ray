#ifndef DELTA_BASIC_LEDGE_LINK_H
#define DELTA_BASIC_LEDGE_LINK_H

#include "rigid_body_link.h"

namespace dphysics {

    class LedgeLink : public RigidBodyLink {
    public:
        LedgeLink();
        ~LedgeLink();

        virtual int GenerateCollisions(Collision *collisionArray);

        ysVector GetAnchor() const { return m_anchorPoint; }
        void SetAnchor(const ysVector &anchor) { m_anchorPoint = anchor; }

        ysVector GetGripLocal() const { return m_gripLocal; }
        void SetGripLocal(const ysVector &grip) { m_gripLocal = grip; }

    protected:
        ysVector m_anchorPoint;
        ysVector m_gripLocal;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_LEDGE_LINK_H */
