#ifndef DIELECTRIC_MEDIA_INTERFACE_H
#define DIELECTRIC_MEDIA_INTERFACE_H

#include <media_interface.h>

namespace manta {

	class DielectricMediaInterface : public MediaInterface {
	public:
		DielectricMediaInterface();
		~DielectricMediaInterface();

		void setIorIncident(math::real iorIncident) { m_iorIncident = iorIncident; }
		math::real getIorIncident() const { return m_iorIncident; }

		void setIorTransmitted(math::real iorTransmitted) { m_iorTransmitted = iorTransmitted; }
		math::real getIorTransmitted() const { return m_iorTransmitted; }

		virtual math::real fresnelTerm(const math::Vector &i, const math::Vector &m, DIRECTION d) const;
		virtual math::real fresnelTerm(math::real cosThetaI, math::real *pdf, DIRECTION d) const;

	protected:
		math::real m_iorIncident;
		math::real m_iorTransmitted;
	};

} /* namespace manta */

#endif /* DIELECTRIC_MEDIA_INTERFACE */
