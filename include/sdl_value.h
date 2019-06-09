#ifndef SDL_VALUE_H
#define SDL_VALUE_H

#include <sdl_parser_structure.h>

namespace manta {

	class SdlValue : public SdlParserStructure {
	public:
		enum VALUE_TYPE {
			CONSTANT_FLOAT,
			CONSTANT_INT,
			CONSTANT_STRING,
			CONSTANT_LABEL,
			CONSTANT_BOOL,
			BINARY_OPERATION,
			NODE_REF
		};

	public:
		SdlValue(VALUE_TYPE type);
		virtual ~SdlValue();

		VALUE_TYPE getType() const { return m_type; }

	private:
		VALUE_TYPE m_type;
	};

} /* namespace manta */

#endif /* SDL_VALUE_H */