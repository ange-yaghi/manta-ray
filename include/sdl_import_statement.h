#ifndef SDL_IMPORT_STATEMENT_H
#define SDL_IMPORT_STATEMENT_H

#include <string>

#include <sdl_parser_structure.h>
#include <sdl_token_info.h>
#include <sdl_visibility.h>

namespace manta {

	class SdlCompilationUnit;

	class SdlImportStatement : public SdlParserStructure {
	public:
		SdlImportStatement(const SdlTokenInfo_string &libName);
		~SdlImportStatement();

		std::string getLibName() const { return m_libName.data; }

		void setUnit(SdlCompilationUnit *unit) { m_unit = unit; }
		SdlCompilationUnit *getUnit() const { return m_unit; }

		void setShortName(const SdlTokenInfo_string &shortName) { m_shortName = shortName; }
		bool hasShortName() const { return m_shortName.specified; }
		std::string getShortName() const { return m_shortName.data; }

	protected:
		SdlTokenInfo_string m_shortName;
		SdlTokenInfo_string m_libName;
		SdlCompilationUnit *m_unit;
	};

} /* namespace manta */

#endif /* IMPORT_STATEMENT */
