#ifndef PIRANHA_IR_INPUT_CONNECTION_H
#define PIRANHA_IR_INPUT_CONNECTION_H

#include "ir_parser_structure.h"

#include <string>

namespace piranha {

	class IrAttributeDefinition;

	class IrInputConnection : public IrParserStructure {
	public:
		IrInputConnection();
		~IrInputConnection();

		void setMember(const std::string &member) { m_member = member; }
		std::string getMember() const { return m_member; }

		IrAttributeDefinition *getInputAttribute() const { return m_inputAttribute; }
		void setInputAttribute(IrAttributeDefinition *attribute) { m_inputAttribute = attribute; }

	protected:
		std::string m_member;
		IrAttributeDefinition *m_inputAttribute;
	};

} /* namespace piranha */

#endif /* PIRANHA_IR_INPUT_CONNECTION_H */
