#ifndef PHONG_EMITTER_H
#define PHONG_EMITTER_H

#include <ray_emitter.h>

namespace manta {

	class PhongEmitter : public RayEmitter {
	public:
		PhongEmitter();
		virtual ~PhongEmitter();

		virtual void generateRays();

		void setFaceNormal(const math::Vector &normal) { m_faceNormal = normal; }
		math::Vector getFaceNormal() const { return m_faceNormal; }

		void setVertexNormal(const math::Vector &normal) { m_vertexNormal = normal; }
		math::Vector getVertexNormal() const { return m_vertexNormal; }

		void setSampleCount(int samples) { m_samples = samples; }
		int getSampleCount() const { return m_samples; }

		void setMeta(int meta) { m_meta = meta; }
		int getMeta() const { return m_meta; }

		void setPower(math::real power) { m_power = power; }
		math::real getPower() const { return m_power; }

		void setIncident(const math::Vector &i) { m_incident = math::negate(i); }
		math::Vector getIncident() const { return m_incident; }

	protected:
		math::real g1(const math::Vector &i, const math::Vector &m) const;
		math::Vector generateMicrofacet(const math::Vector &u, const math::Vector &v) const;
		math::real calculateDistribution(const math::Vector &m) const;
		math::real calculateG(const math::Vector &o, const math::Vector &m) const;
		math::real calculateFresnel() const;

		math::real calculateReflectance(const math::Vector &o, const math::Vector &m) const;
		math::real calculateWeight(const math::Vector &m, const math::Vector &o) const;

		math::Vector reflect(const math::Vector &v, const math::Vector &n) const;
		
		math::Vector m_incident;
		math::Vector m_faceNormal;
		math::Vector m_vertexNormal;
		math::real m_power;

		int m_meta;
		int m_samples;
	};

} /* namespace manta */

#endif /* HEMI_MONTE_CARLO_EMITTER_H */