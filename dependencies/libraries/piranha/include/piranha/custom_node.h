#ifndef PIRANHA_CUSTOM_NODE_H
#define PIRANHA_CUSTOM_NODE_H

#include "node.h"

namespace piranha {

	class CustomNode : public Node {
	public:
		struct CustomPort {
			NodeOutput *port;
			std::string name;
			bool primary;
		};

	public:
		CustomNode();
		~CustomNode();

		void addCustomOutput(NodeOutput *output, const std::string &name, bool primary);
		void addCustomInput(const std::string &name);

		int getCustomInputCount() const { return (int)m_customInputs.size(); }
		int getCustomOutputCount() const { return (int)m_customOutputs.size(); }

	protected:
		virtual void _initialize();

		virtual void registerInputs();
		virtual void registerOutputs();

	protected:
		std::vector<CustomPort *> m_customInputs;
		std::vector<CustomPort *> m_customOutputs;
	};

} /* namespace piranha */

#endif /* PIRANHA_CUSTOM_NODE_H */
