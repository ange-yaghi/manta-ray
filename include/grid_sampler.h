#ifndef GRID_SAMPLER_H
#define GRID_SAMPLER_H

#include <sampler_2d.h>

namespace manta {

	class GridSampler : public Sampler2D {
	public:
		GridSampler();
		virtual ~GridSampler();

		virtual void generateSamples(int sampleCount, math::Vector *target) const;
		virtual int getTotalSampleCount(int sampleCount) const;

		void setGridWidth(int gridWidth) { m_gridWidth = gridWidth; }
		int getGridWidth() const { return m_gridWidth; }

	protected:
		int m_gridWidth;
	};

} /* namespace manta */

#endif /* GRID_SAMPLER_H */
