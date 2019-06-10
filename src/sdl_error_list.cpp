#include <sdl_error_list.h>

#include <sdl_compilation_error.h>

manta::SdlErrorList::SdlErrorList() {
	/* void */
}

manta::SdlErrorList::~SdlErrorList() {
	/* void */
}

void manta::SdlErrorList::addCompilationError(SdlCompilationError *err) {
	m_compilationErrors.push_back(err);
}

void manta::SdlErrorList::destroy() {
	int errorCount = getErrorCount();

	for (int i = 0; i < errorCount; i++) {
		delete m_compilationErrors[i];
	}

	m_compilationErrors.clear();
}
