#ifndef PIRANHA_SINGLE_STRING_NODE_OUTPUT_H
#define PIRANHA_SINGLE_STRING_NODE_OUTPUT_H

#include "string_node_output.h"

#include <string>

namespace piranha {

	class SingleStringNodeOutput : public StringNodeOutput {
	public:
		SingleStringNodeOutput();
		virtual ~SingleStringNodeOutput();

		virtual void fullCompute(void *target) const;

		std::string getValue() const { return m_value; }
		void setValue(const std::string &v) { m_value = v; }

	protected:
		std::string m_value;
	};

} /* namespace piranha */

#endif /* PIRANHA_SINGLE_STRING_NODE_OUTPUT_H */
