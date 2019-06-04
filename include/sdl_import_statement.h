#ifndef SDL_IMPORT_STATEMENT_H
#define SDL_IMPORT_STATEMENT_H

#include <string>

#include <sdl_parser_structure.h>
#include <sdl_token_info.h>

namespace manta {

	class SdlImportStatement : public SdlParserStructure {
	public:
		SdlImportStatement(const SdlTokenInfo_string &libName);
		~SdlImportStatement();

		std::string getLibName() const { return m_libName.data; }

	protected:
		SdlTokenInfo_string m_libName;
	};

} /* namespace manta */

#endif /* IMPORT_STATEMENT */
