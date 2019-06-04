#ifndef SDL_NODE_H
#define SDL_NODE_H

#include <sdl_parser_structure.h>

#include <sdl_token_info.h>

namespace manta {

	class SdlAttributeList;

	class SdlNode : public SdlParserStructure {
	public:
		SdlNode();
		SdlNode(const SdlTokenInfo_string &type, const SdlTokenInfo_string &name, SdlAttributeList *attributes);
		SdlNode(const SdlTokenInfo_string &type, SdlAttributeList *attributes);
		~SdlNode();

		const std::string &getType() const { return m_type.data; }
		const std::string &getName() const { return m_name.data; }

		void setAttributes(SdlAttributeList *list) { m_attributes = list; }
		SdlAttributeList *getAttributes() const { return m_attributes; }

	protected:
		SdlTokenInfo_string m_type;
		SdlTokenInfo_string m_name;

		SdlAttributeList *m_attributes;
	};

} /* namespace manta */

#endif /* SDL_NODE_H */
