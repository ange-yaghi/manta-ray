#ifndef SAMPLER_2D_H
#define SAMPLER_2D_H

#include <manta_math.h>

namespace manta {

	class StackAllocator;

	class Sampler2D {
	public:
		Sampler2D();
		~Sampler2D();

		virtual void generateSamples(int sampleCount, math::Vector *target) const = 0;
		virtual int getTotalSampleCount(int sampleCount) const = 0;

		void setAxis1(const math::Vector &axis1) { m_axis1 = axis1; }
		math::Vector getAxis1() const { return m_axis1; }

		void setAxis2(const math::Vector &axis2) { m_axis2 = axis2; }
		math::Vector getAxis2() const { return m_axis2; }

		void setBoundaryWidth(math::real boundary) { m_boundaryWidth = boundary; }
		math::real getBoundaryWidth() const { return m_boundaryWidth; }

		void setBoundaryHeight(math::real boundary) { m_boundaryHeight = boundary; }
		math::real getBoundaryHeight() const { return m_boundaryHeight; }

	protected:
		math::Vector m_axis1;
		math::Vector m_axis2;
		math::real m_boundaryWidth;
		math::real m_boundaryHeight;
	};

} /* namespace manta */

#endif /* SAMPLER_2D_H */
