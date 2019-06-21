#ifndef SDL_ATTRIBUTE_DEFINITION_H
#define SDL_ATTRIBUTE_DEFINITION_H

#include <sdl_parser_structure.h>

#include <sdl_token_info.h>

namespace manta {

	class SdlValue;
	class SdlInputConnection;
	class SdlCompilationError;

	class SdlAttributeDefinition : public SdlParserStructure {
	public:
		enum DIRECTION {
			INPUT,
			OUTPUT
		};

	public:
		SdlAttributeDefinition(const SdlTokenInfo_string &directionToken, 
			const SdlTokenInfo_string &name, DIRECTION dir);
		virtual ~SdlAttributeDefinition();

		const SdlTokenInfo *getNameToken() const { return &m_name; }
		std::string getName() const { return m_name.data; }

		void setDefaultValue(SdlValue *value);
		SdlValue *getDefaultValue() const { return m_defaultValue; }

		void setDefaultToken(const SdlTokenInfo_string &name);
		const SdlTokenInfo *getDefaultToken() const { return (m_isDefault) ? &m_defaultToken : nullptr; }
		void setDefault(bool isDefault) { m_isDefault = isDefault; }
		bool isDefault() const { return m_isDefault; }

		const SdlTokenInfo *getDirectionToken() const { return &m_directionToken; }
		void setDirection(DIRECTION direction) { m_direction = direction; }
		DIRECTION getDirection() const { return m_direction; }

		SdlInputConnection *getImpliedMember(int i) const { return m_impliedMembers[i]; }
		int getImpliedMemberCount() const { return (int)m_impliedMembers.size(); }
		SdlInputConnection *getImpliedMember(const std::string &name) const;

		virtual SdlParserStructure *resolveLocalName(const std::string &name) const;
		virtual SdlParserStructure *resolveName(const std::string &name) const;

		virtual SdlParserStructure *getImmediateReference(SdlParserStructure *inputContext, SdlCompilationError **err);

		virtual bool isConnectionPoint() const { return m_direction == INPUT; }

	protected:
		SdlTokenInfo_string m_directionToken;
		SdlTokenInfo_string m_defaultToken;
		SdlTokenInfo_string m_name;
		SdlValue *m_defaultValue;

		DIRECTION m_direction;
		bool m_isDefault;

		std::vector<SdlInputConnection *> m_impliedMembers;
	};

} /* namespace manta */

#endif /* SDL_ATTRIBUTE_DEFINITION_H */
