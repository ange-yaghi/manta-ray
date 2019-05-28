#ifndef SDL_VALUE_NODE_REF_H
#define SDL_VALUE_NODE_REF_H

#include <string>

#include <sdl_value.h>

namespace manta {

	class SdlNode;

	class SdlValueNodeRef : public SdlValue {
	public:
		SdlValueNodeRef(SdlNode *nodeRef);
		virtual ~SdlValueNodeRef();

		void setNodeRef(SdlNode *nodeRef) { m_nodeRef = nodeRef; }
		SdlNode *getNodeRef() const { return m_nodeRef; }

	protected:
		SdlNode *m_nodeRef;
	};

} /* namespace manta */

#endif /* SDL_VALUE_NODE_REF_H */
