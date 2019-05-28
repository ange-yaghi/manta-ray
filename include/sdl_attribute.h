#ifndef SDL_ATTRIBUTE_H
#define SDL_ATTRIBUTE_H

#include <string>

namespace manta {

	class SdlValue;

	class SdlAttribute {
	public:
		SdlAttribute();
		SdlAttribute(const std::string &name, SdlValue *value);
		~SdlAttribute();

		void setName(const std::string &name) { m_name = name; }
		std::string getName() const { return m_name; }

		void setValue(SdlValue *value) { m_value = value; }
		SdlValue *getValue() const { return m_value; }

	protected:
		std::string m_name;
		SdlValue *m_value;
	};

} /* namespace manta */

#endif /* SDL_ATTRIBUTE_H */
