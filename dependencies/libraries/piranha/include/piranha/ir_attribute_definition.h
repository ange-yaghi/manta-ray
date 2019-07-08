#ifndef PIRANHA_IR_ATTRIBUTE_DEFINITION_H
#define PIRANHA_IR_ATTRIBUTE_DEFINITION_H

#include "ir_parser_structure.h"

#include "ir_token_info.h"

namespace piranha {

	class IrValue;
	class IrInputConnection;
	class IrCompilationError;
	class IrNodeDefinition;

	class IrAttributeDefinition : public IrParserStructure {
	public:
		enum DIRECTION {
			INPUT,
			OUTPUT
		};

	public:
		IrAttributeDefinition(const IrTokenInfo_string &directionToken, 
			const IrTokenInfo_string &name, DIRECTION dir);
		IrAttributeDefinition(const IrTokenInfo_string &name);
		virtual ~IrAttributeDefinition();

		const IrTokenInfo *getNameToken() const { return &m_name; }
		std::string getName() const { return m_name.data; }

		void setDefaultValue(IrValue *value);
		IrValue *getDefaultValue() const { return m_defaultValue; }

		void setDefaultToken(const IrTokenInfo_string &name);
		const IrTokenInfo *getDefaultToken() const { return (m_isDefault) ? &m_defaultToken : nullptr; }
		void setDefault(bool isDefault) { m_isDefault = isDefault; }
		bool isDefault() const { return m_isDefault; }

		const IrTokenInfo *getDirectionToken() const { return &m_directionToken; }
		void setDirection(DIRECTION direction) { m_direction = direction; }
		DIRECTION getDirection() const { return m_direction; }

		IrInputConnection *getImpliedMember(int i) const { return m_impliedMembers[i]; }
		int getImpliedMemberCount() const { return (int)m_impliedMembers.size(); }
		IrInputConnection *getImpliedMember(const std::string &name) const;

		virtual IrParserStructure *getImmediateReference(const IrReferenceQuery &query, IrReferenceInfo *output);

		virtual bool isInputPoint() const { return m_direction == INPUT; }

		void setTypeName(const IrTokenInfoSet<std::string, 2> &typeInfo) { m_typeInfo = typeInfo; }

		IrNodeDefinition *getTypeDefinition() const { return m_typeDefinition; }

	protected:
		IrTokenInfo_string m_directionToken;
		IrTokenInfo_string m_defaultToken;
		IrTokenInfo_string m_name;
		IrTokenInfoSet<std::string, 2> m_typeInfo;

		IrValue *m_defaultValue;

		DIRECTION m_direction;
		bool m_isDefault;

		std::vector<IrInputConnection *> m_impliedMembers;

		// Resolution stage
	protected:
		IrNodeDefinition *m_typeDefinition;
	};

} /* namespace piranha */

#endif /* PIRANHA_IR_ATTRIBUTE_DEFINITION_H */
