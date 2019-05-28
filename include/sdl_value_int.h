#ifndef SDL_VALUE_INT_H
#define SDL_VALUE_INT_H

#include <sdl_value.h>

namespace manta {

	class SdlValueInt : public SdlValue {
	public:
		SdlValueInt(int *values, int n);
		virtual ~SdlValueInt();

		int getLength() const { return m_length; }
		int get(int index) const { return m_values[index]; }

		void initialize(int length);

	protected:
		int *m_values;
		int m_length;
	};

} /* namespace manta */

#endif /* SDL_VALUE_INT_H */
