#ifndef SDL_ATTRIBUTE_DEFINITION_LIST_H
#define SDL_ATTRIBUTE_DEFINITION_LIST_H

#include <sdl_parser_structure.h>

#include <sdl_attribute_definition.h>

#include <vector>
#include <string>

namespace manta {

	class SdlAttributeDefinitionList : public SdlParserStructure {
	public:
		SdlAttributeDefinitionList();
		~SdlAttributeDefinitionList();

		void addDefinition(SdlAttributeDefinition *definition);
		SdlAttributeDefinition *getDefinition(int index) const { return m_definitions[index]; }
		int getDefinitionCount() const { return (int)m_definitions.size(); }

		SdlAttributeDefinition *getDefinition(int index, SdlAttributeDefinition::DIRECTION direction) const;
		SdlAttributeDefinition *getInputDefinition(int index) const;

		SdlAttributeDefinition *getOutputDefinition(int index) const;
		SdlAttributeDefinition *getOutputDefinition(const std::string &name) const;
		SdlAttributeDefinition *getDefaultOutput() const;

		int getCount(SdlAttributeDefinition::DIRECTION direction) const;
		int getInputCount() const;
		int getOutputCount() const;

	protected:
		std::vector<SdlAttributeDefinition *> m_definitions;
	};

} /* namespace manta */

#endif /* SDL_ATTRIBUTE_DEFINITION_LIST_H */
