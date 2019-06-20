#ifndef SDL_ATTRIBUTE_H
#define SDL_ATTRIBUTE_H

#include <sdl_parser_structure.h>

#include <sdl_token_info.h>

namespace manta {

	class SdlValue;
	class SdlAttributeDefinition;

	class SdlAttribute : public SdlParserStructure {
	public:
		SdlAttribute();
		SdlAttribute(const SdlTokenInfo_string &name, SdlValue *value);
		SdlAttribute(SdlValue *value);
		~SdlAttribute();

		std::string getName() const { return m_name.data; }

		void setValue(SdlValue *value);
		SdlValue *getValue() const { return m_value; }

		void setPosition(int position) { m_position = position; }
		int getPosition() const { return m_position; }

		// If no name is specified positional notation is assumed
		bool isPositional() const { return getName() == ""; }

	protected:
		SdlTokenInfo_string m_name;
		SdlValue *m_value;

		int m_position;

		// Resolution stage
	public:
		void setAttributeDefinition(SdlAttributeDefinition *definition) { m_definition = definition; }
		SdlAttributeDefinition *getAttributeDefinition() const { return m_definition; }

	protected:
		SdlAttributeDefinition *m_definition;
	};

} /* namespace manta */

#endif /* SDL_ATTRIBUTE_H */
