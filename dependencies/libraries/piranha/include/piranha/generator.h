#ifndef PIRANHA_IR_GENERATOR_H
#define PIRANHA_IR_GENERATOR_H

#include <string>

namespace piranha {

	class Node;
	class IrNode;

	class IrGenerator {
	public:
		enum BuiltinCode {
			BUILTIN_CONSOLE_WRITE
		};

		struct BuiltinType {
			BuiltinCode code;
			std::string name;
		};

	public:
		IrGenerator();
		~IrGenerator();

		Node *generateNode(IrNode *reference) const;
	};

} /* namespace piranha */

#endif /* PIRANHA_IR_GENERATOR_H */
