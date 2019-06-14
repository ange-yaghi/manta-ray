#ifndef SDL_NODE_H
#define SDL_NODE_H

#include <sdl_parser_structure.h>

#include <sdl_token_info.h>

namespace manta {

	class SdlAttributeList;
	class SdlNodeDefinition;
	class SdlCompilationUnit;

	class SdlNode : public SdlParserStructure {
	public:
		SdlNode();
		SdlNode(const SdlTokenInfo_string &type, const SdlTokenInfo_string &name, SdlAttributeList *attributes);
		SdlNode(const SdlTokenInfo_string &type, SdlAttributeList *attributes);
		~SdlNode();

		SdlTokenInfo getNameToken() const { return m_name; }

		const std::string &getType() const { return m_type.data; }
		const std::string &getName() const { return m_name.data; }

		void setAttributes(SdlAttributeList *list);
		SdlAttributeList *getAttributes() const { return m_attributes; }

		virtual SdlParserStructure *getPublicAttribute(const std::string &name, bool *failed = nullptr);
		virtual SdlParserStructure *getDefaultInterface();

	protected:
		SdlTokenInfo_string m_type;
		SdlTokenInfo_string m_name;

		SdlAttributeList *m_attributes;

		// Resolution stage
	public:
		SdlNodeDefinition *getDefinition() const { return m_definition; }
		void setDefinition(SdlNodeDefinition *definition) { m_definition = definition; }

		void resolveNodeDefinition(SdlCompilationUnit *unit);
		void resolveAttributeDefinitions(SdlCompilationUnit *unit);

	protected:
		SdlNodeDefinition *m_definition;
	};

} /* namespace manta */

#endif /* SDL_NODE_H */
