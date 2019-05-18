#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <manta_math.h>

namespace manta {

	class Spectrum {
	public:
		Spectrum();
		~Spectrum();

		void initialize(int dataPoints, math::real startWavelength, math::real endWaveLength, 
			math::real *buffer);
		void destroy();

		bool loadCsv(const char *fname);
		void writeCsv(const char *fname) const;

		void clear();
		void add(const Spectrum &b);

		void set(int index, math::real value) { m_values[index] = value; }

		int getPointCount() const { return m_pointCount; }
		math::real getStep() const;

		math::real getValueDiscrete(int index) const { return m_values[index]; }
		math::real getValueContinuous(math::real wavelength) const;

		math::real getStartWavelength() const { return m_startWavelength; }
		math::real getEndWavelength() const { return m_endWaveLength; }

	protected:
		math::real m_startWavelength;
		math::real m_endWaveLength;

		math::real *m_values;
		int m_pointCount;

	protected:
		bool m_externalMemoryAllocation;
	};

} /* namespace manta */

#endif /* SPECTRUM_H */
