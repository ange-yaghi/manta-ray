#ifndef SDL_VALUE_LABEL_H
#define SDL_VALUE_LABEL_H

#include <string>

#include <sdl_value.h>

namespace manta {

	class SdlValueLabel : public SdlValue {
	public:
		SdlValueLabel(const std::string &label);
		virtual ~SdlValueLabel();

		void setLabel(const std::string &label) { m_label = label; }
		std::string getLabel() const { return m_label; }

	protected:
		std::string m_label;
	};

} /* namespace manta */

#endif /* SDL_VALUE_LABEL_H */
