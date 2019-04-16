#include <spectrum.h>

#include <standard_allocator.h>

#include <assert.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

manta::Spectrum::Spectrum() {
	m_values = nullptr;
	m_externalMemoryAllocation = false;

	m_pointCount = 0;
	m_endWaveLength = (math::real)0.0;
	m_startWavelength = (math::real)0.0;
}

manta::Spectrum::~Spectrum() {
	assert(m_values == nullptr);
}

void manta::Spectrum::initialize(int dataPoints, math::real startWavelength, math::real endWaveLength, math::real *buffer) {
	if (buffer != nullptr) {
		m_values = buffer;
		m_externalMemoryAllocation = true;
	}
	else {
		m_values = StandardAllocator::Global()->allocate<math::real>(dataPoints);
		m_externalMemoryAllocation = false;
	}

	m_pointCount = dataPoints;
	m_startWavelength = startWavelength;
	m_endWaveLength = endWaveLength;
}

void manta::Spectrum::destroy() {
	if (!m_externalMemoryAllocation) {
		StandardAllocator::Global()->free(m_values, m_pointCount);

		m_values = nullptr;
	}
	else {
		// It's assumed that some external agent has already
		// taken care of cleaning up the memory
		m_values = nullptr;
	}

	m_endWaveLength = (math::real)0.0;
	m_startWavelength = (math::real)0.0;
	m_externalMemoryAllocation = false;
	m_pointCount = 0;
}

template <char D>
std::istream& delim(std::istream& in) {
	char c;
	if (in >> c && c != D) in.setstate(std::ios_base::failbit);
	return in;
}

bool manta::Spectrum::loadCsv(const char *fname) {
	struct Entry {
		math::real wavelength;
		math::real power;
	};

	std::vector<Entry> tempEntries;

	std::ifstream file(fname);
	std::string line;
	bool success = true;
	while (std::getline(file, line)) {
		std::istringstream ss(line);
		Entry newEntry;

		if (!(ss >>
			newEntry.wavelength >> delim<','> >>
			newEntry.power)) {
			if (!(ss >> delim<','>)) {
				// An error has occurred
				success = false;
				break;
			}
		}

		tempEntries.push_back(newEntry);
	}

	file.close();

	if (!success) {
		return false;
	}

	m_pointCount = (int)tempEntries.size();
	m_values = StandardAllocator::Global()->allocate<math::real>(m_pointCount);
	m_externalMemoryAllocation = false;
	m_startWavelength = tempEntries[0].wavelength;
	m_endWaveLength = tempEntries[m_pointCount - 1].wavelength;

	for (int i = 0; i < m_pointCount; i++) {
		m_values[i] = tempEntries[i].power;
	}

	return true;
}

void manta::Spectrum::writeCsv(const char *fname) const {
	assert(m_values != nullptr);

	std::ofstream file(fname);

	math::real wavelengthStep = getStep();

	for (int i = 0; i < m_pointCount; i++) {
		math::real wavelength = wavelengthStep * i + m_startWavelength;
		math::real power = m_values[i];
		file << wavelength << "," << power << std::endl;
	}

	file.close();
}

void manta::Spectrum::clear() {
	assert(m_values != nullptr);

	for (int i = 0; i < m_pointCount; i++) {
		m_values[i] = (math::real)0.0;
	}
}

void manta::Spectrum::add(const Spectrum &b) {
	assert(b.m_endWaveLength == m_endWaveLength);
	assert(b.m_startWavelength == m_startWavelength);
	assert(b.m_pointCount == m_pointCount);
	assert(m_values != nullptr);
	assert(b.m_values != nullptr);

	for (int i = 0; i < m_pointCount; i++) {
		m_values[i] += b.m_values[i];
	}
}

manta::math::real manta::Spectrum::getValueDiscrete(int index) const {
	assert(m_values != nullptr);

	return m_values[index];
}

manta::math::real manta::Spectrum::getValueContinuous(math::real wavelength) const {
	assert(m_values != nullptr);

	math::real step = getStep();
	math::real indexApprox = (wavelength - m_startWavelength) / step;
	int prevIndex = (int)indexApprox;

	if (prevIndex < 0) return (math::real)0.0;
	if (prevIndex >= m_pointCount - 1) return (math::real)0.0;

	int nextIndex = prevIndex + 1;
	math::real s = indexApprox - (math::real)prevIndex;

	return m_values[prevIndex] * (1 - s) + m_values[nextIndex] * s;
}
