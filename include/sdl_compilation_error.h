#ifndef SDL_COMPILATION_ERROR_H
#define SDL_COMPILATION_ERROR_H

#include <sdl_token_info.h>

#include <string>

namespace manta {

	class SdlCompilationUnit;

	class SdlCompilationError {
	public:
		struct ErrorCode {
			std::string stage;
			std::string code;
			std::string info;
		};

	public:
		SdlCompilationError(const SdlTokenInfo &location, const ErrorCode &code);
		~SdlCompilationError();

		const SdlTokenInfo *getErrorLocation() const { return &m_errorLocation; }
		void setErrorLocation(SdlTokenInfo &loc) { m_errorLocation = loc; }

		void setErrorCode(const ErrorCode &code) { m_code = code; }
		const ErrorCode &getErrorCode() const { return m_code; }

		void setCompilationUnit(SdlCompilationUnit *unit) { m_unit = unit; }
		SdlCompilationUnit *getCompilationUnit() const { return m_unit; }

	protected:
		SdlTokenInfo m_errorLocation;
		ErrorCode m_code;
		SdlCompilationUnit *m_unit;
	};

} /* namespace manta */

#endif /* SDL_COMPILATION_ERROR_H */
