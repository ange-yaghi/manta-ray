#ifndef MANTARAY_GRID_SAMPLER_H
#define MANTARAY_GRID_SAMPLER_H

#include "sampler_2d.h"

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
        virtual void _initialize();
        virtual void _evaluate();
        virtual void registerInputs();

        piranha::pNodeInput m_gridWidthInput;

	protected:
		int m_gridWidth;
	};

} /* namespace manta */

#endif /* MANTARAY_GRID_SAMPLER_H */
