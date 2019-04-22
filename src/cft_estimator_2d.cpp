#include <cft_estimator_2d.h>
#include <complex_map_2d.h>

#include <assert.h>

manta::CftEstimator2D::CftEstimator2D() {
	m_physicalWidth = (math::real_d)0.0;
	m_physicalHeight = (math::real_d)0.0;

	m_horizontalSamples = 0;
	m_verticalSamples = 0;
}

manta::CftEstimator2D::~CftEstimator2D() {
	
}

void manta::CftEstimator2D::initialize(const ComplexMap2D *spatialFunction, math::real_d phyiscalWidth, math::real_d physicalHeight) {
	m_physicalWidth = phyiscalWidth;
	m_physicalHeight = physicalHeight;

	m_verticalSamples = spatialFunction->getWidth();
	m_horizontalSamples = spatialFunction->getHeight();

	ComplexMap2D dftApprox;
	spatialFunction->fft_multithreaded(&dftApprox, 12);

	dftApprox.cft(&m_discreteApproximation, phyiscalWidth, physicalHeight);
	dftApprox.destroy();

	m_mipMap.initialize(&m_discreteApproximation);
}

void manta::CftEstimator2D::destroy() {
	m_discreteApproximation.destroy();
	m_mipMap.destroy();
}

manta::math::Complex manta::CftEstimator2D::sample(math::real_d freq_x, math::real_d freq_y, math::real_d w) const {
	math::real_d k_x, k_y;

	k_x = freq_x * m_physicalWidth;
	k_y = freq_y * m_physicalHeight;

	// Wrap indices
	k_x = (k_x < 0) ? m_horizontalSamples + k_x : k_x;
	k_y = (k_y < 0) ? m_verticalSamples + k_y : k_y;

	return m_discreteApproximation.sampleDiscrete((int)k_x, (int)k_y);

	// Calculate uv coordinates
	/*
	math::real_d u, v, w_scaled;
	u = (freq_x * m_physicalWidth);
	v = (freq_y * m_physicalHeight);

	u = (u < 0) ? (math::real_d)m_horizontalSamples + u : u;
	v = (v < 0) ? (math::real_d)m_verticalSamples + v : v;

	u /= m_horizontalSamples;
	v /= m_verticalSamples;

	w_scaled = (w * m_physicalWidth) / m_discreteApproximation.getWidth();

	return m_mipMap.sample(u, v, w_scaled);*/
}

manta::math::real_d manta::CftEstimator2D::getHorizontalFreqRange() const {
	return getFreqRange(m_horizontalSamples, m_physicalWidth);
}

manta::math::real_d manta::CftEstimator2D::getVerticalFreqRange() const {
	return getFreqRange(m_verticalSamples, m_physicalHeight);
}

manta::math::real_d manta::CftEstimator2D::getHorizontalFreqStep() const {
	return getFreqStep(m_physicalWidth);
}

manta::math::real_d manta::CftEstimator2D::getVerticalFreqStep() const {
	return getFreqStep(m_physicalHeight);
}

manta::math::real_d manta::CftEstimator2D::getFreqRange(int horizontalSamples, math::real_d physicalWidth) {
	math::real_d maxFreq = (horizontalSamples / 2) / physicalWidth;
	return maxFreq;
}

manta::math::real_d manta::CftEstimator2D::getFreqStep(math::real_d physicalWidth) {
	return 1 / physicalWidth;
}

manta::math::real_d manta::CftEstimator2D::getMinPhysicalDim(math::real_d freqStep, math::real_d minDim) {
	math::real_d perfect = 1 / freqStep;
	return (perfect > minDim) ? perfect : minDim;
}

int manta::CftEstimator2D::getMinSamples(math::real_d maxFreq, math::real_d physicalDim, int maxSamples) {
	// Calculate a rough approximation based on the maximum frequency
	int firstApprox = (int)(maxFreq * physicalDim * 2);

	// Round to a power of 2
	int minSamples = 1;
	while (minSamples < firstApprox && (minSamples < maxSamples || maxSamples < 0)) {
		minSamples <<= 1;
	}

	return minSamples;
}
