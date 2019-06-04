#ifndef SDL_ATTRIBUTE_DEFINITION_LIST_H
#define SDL_ATTRIBUTE_DEFINITION_LIST_H

#include <sdl_parser_structure.h>

#include <vector>

namespace manta {

	class SdlAttributeDefinition;

	class SdlAttributeDefinitionList : public SdlParserStructure {
	public:
		SdlAttributeDefinitionList();
		~SdlAttributeDefinitionList();

		void addDefinition(SdlAttributeDefinition *definition);
		SdlAttributeDefinition *getDefinition(int index) const { return m_definitions[index]; }
		int getDefinitionCount() const { return (int)m_definitions.size(); }

	protected:
		std::vector<SdlAttributeDefinition *> m_definitions;
	};

} /* namespace manta */

#endif /* SDL_ATTRIBUTE_DEFINITION_LIST_H */
