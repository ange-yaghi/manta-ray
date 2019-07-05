#ifndef SDL_GENERATOR_H
#define SDL_GENERATOR_H

#include <string>

namespace manta {

	class Node;
	class SdlNode;

	class SdlGenerator {
	public:
		enum BuiltinCode {
			BUILTIN_CONSOLE_WRITE
		};

		struct BuiltinType {
			BuiltinCode code;
			std::string name;
		};

	public:
		SdlGenerator();
		~SdlGenerator();

		Node *generateNode(SdlNode *reference) const;
	};

} /* namespace manta */

#endif /* SDL_GENERATOR_H */
