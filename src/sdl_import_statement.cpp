#include <sdl_import_statement.h>

manta::SdlImportStatement::SdlImportStatement(const SdlTokenInfo_string &libName) : m_libName(libName) {
	registerToken(&libName);
}

manta::SdlImportStatement::~SdlImportStatement() {
	/* void */
}
