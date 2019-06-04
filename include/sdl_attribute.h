#ifndef SDL_ATTRIBUTE_H
#define SDL_ATTRIBUTE_H

#include <sdl_parser_structure.h>

#include <sdl_token_info.h>

namespace manta {

	class SdlValue;

	class SdlAttribute : public SdlParserStructure {
	public:
		SdlAttribute();
		SdlAttribute(const SdlTokenInfo_string &name, SdlValue *value);
		SdlAttribute(SdlValue *value);
		~SdlAttribute();

		std::string getName() const { return m_name.data; }

		void setValue(SdlValue *value) { m_value = value; }
		SdlValue *getValue() const { return m_value; }

	protected:
		SdlTokenInfo_string m_name;
		SdlValue *m_value;
	};

} /* namespace manta */

#endif /* SDL_ATTRIBUTE_H */
