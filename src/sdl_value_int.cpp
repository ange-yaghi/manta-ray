#include <sdl_value_int.h>

#include <string.h>

manta::SdlValueInt::SdlValueInt(int *values, int n) : SdlValue(CONSTANT_INT) {
	m_values = new int[n];
	m_length = n;

	memcpy((void *)m_values, values, sizeof(int) * n);
}

manta::SdlValueInt::~SdlValueInt() {
	/* void */
}
