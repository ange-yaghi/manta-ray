#ifndef SDL_INPUT_CONNECTION_H
#define SDL_INPUT_CONNECTION_H

#include <sdl_parser_structure.h>

#include <string>

namespace manta {

	class SdlAttributeDefinition;

	class SdlInputConnection : public SdlParserStructure {
	public:
		SdlInputConnection();
		~SdlInputConnection();

		void setMember(const std::string &member) { m_member = member; }
		std::string getMember() const { return m_member; }

		SdlAttributeDefinition *getInputAttribute() const { return m_inputAttribute; }
		void setInputAttribute(SdlAttributeDefinition *attribute) { m_inputAttribute = attribute; }

	protected:
		std::string m_member;
		SdlAttributeDefinition *m_inputAttribute;
	};

} /* namespace manta */

#endif /* SDL_INPUT_CONNECTION_H */
