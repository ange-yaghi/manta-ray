#ifndef RAY_CONTAINER_H
#define RAY_CONTAINER_H

#include <manta_math.h>

namespace manta {

	// Forward declarations
	class LightRay;
	class StackAllocator;

	class RayContainer {
	public:
		RayContainer();
		virtual ~RayContainer();

		LightRay *getRays() const { return m_rays; }
		int getRayCount() const { return m_rayCount; }

		void initializeRays(int count);
		void destroyRays();

		int getDegree() const { return m_degree; }
		void setDegree(int degree) { m_degree = degree; }

		int getSamplesPerRay() const { return m_samplesPerRay; }
		void setSamplesPerRay(int samples) { m_samplesPerRay = samples; }

		void setStackAllocator(StackAllocator *allocator) { m_stackAllocator = allocator; }
		StackAllocator *getStackAllocator() const { return m_stackAllocator; }

		virtual void calculateIntensity();
		math::Vector weightedAverage() const;
		math::Vector getIntensity() const { return m_intensity; }

	private:
		int m_degree;
		int m_rayCount;
		int m_samplesPerRay;

		LightRay *m_rays;

	protected:
		// Read-only intensity value
		math::Vector m_intensity;

	private:
		StackAllocator *m_stackAllocator;
	};

} /* namespace manta */

#endif /* RAY_CONTAINER_H */
