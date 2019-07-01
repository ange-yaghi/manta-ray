#ifndef SDL_GENERIC_VALUE_H
#define SDL_GENERIC_VALUE_H

#include <sdl_value.h>

namespace manta {

	class SdlNodeDefinition;

	class SdlGenericValue : public SdlValue {
	public:
		SdlGenericValue(const SdlTokenInfoSet<std::string, 2> &type);
		~SdlGenericValue();

		SdlNodeDefinition *getTypeDefinition() const { return m_typeDefinition; }

		virtual bool isGeneric() const { return true; }

	protected:
		SdlTokenInfoSet<std::string, 2> m_typeInfo;

		// Resolution stage
	public:
		virtual SdlParserStructure *resolveLocalName(const std::string &name) const;

	protected:
		virtual void _resolveDefinitions();

		SdlNodeDefinition *m_typeDefinition;
	};

} /* namespace manta */

#endif /* SDL_GENERIC_VALUE_H */
