#ifndef MANTARAY_DIELECTRIC_MEDIA_INTERFACE_H
#define MANTARAY_DIELECTRIC_MEDIA_INTERFACE_H

#include "media_interface.h"

namespace manta {

    class DielectricMediaInterface : public MediaInterface {
    public:
        DielectricMediaInterface();
        virtual ~DielectricMediaInterface();

        void setIorIncident(math::real iorIncident) { m_iorIncident = iorIncident; }
        math::real getIorIncident() const { return m_iorIncident; }

        void setIorTransmitted(math::real iorTransmitted) { m_iorTransmitted = iorTransmitted; }
        math::real getIorTransmitted() const { return m_iorTransmitted; }

        virtual math::real fresnelTerm(
            const math::Vector &i,
            const math::Vector &m,
            Direction d) const;
        virtual math::real fresnelTerm(math::real cosThetaI, Direction d) const;

        virtual math::real ior(Direction d) const;
        virtual math::real no(Direction d) const;
        virtual math::real ni(Direction d) const;

    protected:
        math::real m_iorIncident;
        math::real m_iorTransmitted;

    protected:
        virtual void _evaluate();
        virtual void registerInputs();

        piranha::pNodeInput m_iorIncidentInput;
        piranha::pNodeInput m_iorTransmittedInput;
    };

} /* namespace manta */

#endif /* MANTARAY_DIELECTRIC_MEDIA_INTERFACE */
