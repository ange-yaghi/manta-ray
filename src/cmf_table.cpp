#include <cmf_table.h>

#include <standard_allocator.h>
#include <rgb_space.h>
#include <spectrum.h>

#include <assert.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

manta::CmfTable::CmfTable() {
	m_entries = nullptr;
	m_entryCount = 0;
}

manta::CmfTable::~CmfTable() {
	assert(m_entries == nullptr);
}

template <char D>
std::istream& delim(std::istream& in) {
	char c;
	if (in >> c && c != D) in.setstate(std::ios_base::failbit);
	return in;
}

bool manta::CmfTable::loadCsv(const char *fname) {
	std::vector<Entry> tempEntries;

	std::ifstream file(fname);
	std::string line;
	bool success = true;
	while (std::getline(file, line)) {
		std::istringstream ss(line);
		Entry newEntry;

		if (!(ss >> 
			newEntry.wavelength >> delim<','> >>
			newEntry.colorXyz.x >> delim<','> >>
			newEntry.colorXyz.y >> delim<','> >> newEntry.colorXyz.z)) {
			// An error has occurred
			success = false;
			break;
		}

		tempEntries.push_back(newEntry);
	}

	file.close();

	if (!success) {
		return false;
	}

	m_entryCount = (int)tempEntries.size();
	m_entries = StandardAllocator::Global()->allocate<Entry>(m_entryCount);

	for (int i = 0; i < m_entryCount; i++) {
		m_entries[i] = tempEntries[i];
	}

	return true;
}

void manta::CmfTable::destroy() {
	if (m_entries != nullptr) {
		StandardAllocator::Global()->free(m_entries, m_entryCount);
	}

	m_entryCount = 0;
	m_entries = nullptr;
}

manta::math::Vector manta::CmfTable::pureToRgb(math::real_d wavelength) const {
	assert(m_entries != nullptr);

	Entry entry = sampleXyz(wavelength);
	ColorRgb rgb = RgbSpace::srgb.convertToRgb(entry.colorXyz);
	return math::loadVector((math::real)rgb.x, (math::real)rgb.y, (math::real)rgb.z);
}

manta::math::Vector manta::CmfTable::xyzToRgb(const ColorXyz &color) const {
	assert(m_entries != nullptr);

	ColorRgb rgb = RgbSpace::srgb.convertToRgb(color);
	return math::loadVector((math::real)rgb.x, (math::real)rgb.y, (math::real)rgb.z);
}

manta::CmfTable::Entry manta::CmfTable::sampleXyz(math::real_d wavelength) const {
	assert(m_entries != nullptr);

	// Simple linear search implementation for now
	Entry *high = nullptr, *low = nullptr;
	for (int i = 0; i < m_entryCount; i++) {
		Entry *entry = &m_entries[i];

		if (entry->wavelength > wavelength) high = entry;
		if (entry->wavelength < wavelength) low = entry;

		if (high != nullptr) break;
	}

	// Linearly interpolate between high and low entry
	if (low == nullptr && high == nullptr) return { wavelength, ColorXyz() };
	else if (low == nullptr) return *high;
	else if (high == nullptr) return *low;
	else {
		math::real_d step = high->wavelength - low->wavelength;
		math::real_d d_w = wavelength - low->wavelength;
		math::real_d s = d_w / step;

		return { wavelength, low->colorXyz * (1 - s) + high->colorXyz * s };
	}

	// Shouldn't get here
	return { wavelength, ColorXyz() };
}

manta::ColorXyz manta::CmfTable::spectralToXyz(const Spectrum *spectrum) const {
	math::real_d start = spectrum->getStartWavelength();
	math::real_d end = spectrum->getEndWavelength();
	math::real_d step = spectrum->getStep();
	int dataPoints = spectrum->getPointCount();

	math::real_d reimannSumX = (math::real_d)0.0;
	math::real_d reimannSumY = (math::real_d)0.0;
	math::real_d reimannSumZ = (math::real_d)0.0;
	for (int i = 0; i < dataPoints - 1; i++) {
		math::real_d wavelength = step * i + start;
		ColorXyz response0 = sampleXyz(wavelength).colorXyz;
		ColorXyz response1 = sampleXyz(wavelength + step).colorXyz;

		reimannSumX += (spectrum->getValueDiscrete(i) * response0.x + spectrum->getValueDiscrete(i + 1) * response1.x);
		reimannSumY += (spectrum->getValueDiscrete(i) * response0.y + spectrum->getValueDiscrete(i + 1) * response1.y);
		reimannSumZ += (spectrum->getValueDiscrete(i) * response0.z + spectrum->getValueDiscrete(i + 1) * response1.z);
	}

	reimannSumX *= (step / 2);
	reimannSumY *= (step / 2);
	reimannSumZ *= (step / 2);

	return ColorXyz(reimannSumX, reimannSumY, reimannSumZ);
}
