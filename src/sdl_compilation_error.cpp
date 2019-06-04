#include <sdl_compilation_error.h>

manta::SdlCompilationError::SdlCompilationError(
		const SdlTokenInfo &location, const ErrorCode &code) {
	m_errorLocation = location;
	m_code = code;
}

manta::SdlCompilationError::~SdlCompilationError() {
	/* void */
}
