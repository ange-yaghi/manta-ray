#ifndef SDL_ATTRIBUTE_DEFINITION_H
#define SDL_ATTRIBUTE_DEFINITION_H

#include <sdl_parser_structure.h>

#include <sdl_token_info.h>

namespace manta {

	class SdlValue;
	class SdlInputConnection;
	class SdlCompilationError;
	class SdlNodeDefinition;

	class SdlAttributeDefinition : public SdlParserStructure {
	public:
		enum DIRECTION {
			INPUT,
			OUTPUT
		};

	public:
		SdlAttributeDefinition(const SdlTokenInfo_string &directionToken, 
			const SdlTokenInfo_string &name, DIRECTION dir);
		SdlAttributeDefinition(const SdlTokenInfo_string &name);
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

		virtual SdlParserStructure *getImmediateReference(SdlParserStructure *inputContext, SdlCompilationError **err);

		virtual bool isInputPoint() const { return m_direction == INPUT; }

		void setTypeName(const SdlTokenInfoSet<std::string, 2> &typeInfo) { m_typeInfo = typeInfo; }

		SdlNodeDefinition *getTypeDefinition() const { return m_typeDefinition; }

	protected:
		SdlTokenInfo_string m_directionToken;
		SdlTokenInfo_string m_defaultToken;
		SdlTokenInfo_string m_name;
		SdlTokenInfoSet<std::string, 2> m_typeInfo;

		SdlValue *m_defaultValue;

		DIRECTION m_direction;
		bool m_isDefault;

		std::vector<SdlInputConnection *> m_impliedMembers;

		// Resolution stage
	protected:
		SdlNodeDefinition *m_typeDefinition;
	};

} /* namespace manta */

#endif /* SDL_ATTRIBUTE_DEFINITION_H */
