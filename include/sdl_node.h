#ifndef SDL_NODE_H
#define SDL_NODE_H

#include <string>

namespace manta {

	class SdlAttributeList;

	class SdlNode {
	public:
		SdlNode();
		SdlNode(const std::string &type, const std::string &name, SdlAttributeList *attributes);
		~SdlNode();

		void setType(const std::string &type) { m_type = type; }
		const std::string &getType() const { return m_type; }

		void setName(const std::string &name) { m_name = name; }
		const std::string &getName() const { return m_name; }

		void setAttributes(SdlAttributeList *list) { m_attributes = list; }
		SdlAttributeList *getAttributes() const { return m_attributes; }

	protected:
		std::string m_type;
		std::string m_name;

		SdlAttributeList *m_attributes;
	};

} /* namespace manta */

#endif /* SDL_NODE_H */
