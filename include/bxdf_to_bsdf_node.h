#ifndef MANTARAY_BXDF_TO_BSDF_NODE_H
#define MANTARAY_BXDF_TO_BSDF_NODE_H

#include "bsdf.h"

namespace manta {

    class BxdfToBsdfNode : public ObjectReferenceNode<BSDF> {
    public:
        BxdfToBsdfNode();
        virtual ~BxdfToBsdfNode();

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual void registerInputs();

        piranha::pNodeInput m_bxdfInput;

    protected:
        BSDF m_bsdf;
    };

} /* namespace manta */

#endif /* MANTARAY_BXDF_TO_BSDF_NODE_H */
