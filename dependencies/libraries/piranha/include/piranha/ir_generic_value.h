#ifndef PIRANHA_IR_GENERIC_VALUE_H
#define PIRANHA_IR_GENERIC_VALUE_H

#include "ir_value.h"

namespace piranha {

	class IrNodeDefinition;

	class IrGenericValue : public IrValue {
	public:
		IrGenericValue(const IrTokenInfoSet<std::string, 2> &type);
		~IrGenericValue();

		IrNodeDefinition *getTypeDefinition() const { return m_typeDefinition; }

		virtual bool isGeneric() const { return true; }

	protected:
		IrTokenInfoSet<std::string, 2> m_typeInfo;

		// Resolution stage
	public:
		virtual IrParserStructure *resolveLocalName(const std::string &name) const;

	protected:
		virtual void _resolveDefinitions();

		IrNodeDefinition *m_typeDefinition;
	};

} /* namespace piranha */

#endif /* PIRANHA_IR_GENERIC_VALUE_H */
