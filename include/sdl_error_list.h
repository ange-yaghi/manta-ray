#ifndef SDL_ERROR_LIST_H
#define SDL_ERROR_LIST_H

#include <vector>

namespace manta {

	class SdlCompilationError;

	class SdlErrorList {
	public:
		SdlErrorList();
		~SdlErrorList();

		void addCompilationError(SdlCompilationError *err);
		SdlCompilationError *getCompilationError(int index) { return m_compilationErrors[index]; }
		int getErrorCount() const { return (int)m_compilationErrors.size(); }

		void destroy();

	protected:
		std::vector<SdlCompilationError *> m_compilationErrors;
	};

} /* namespace manta */

#endif /* SDL_ERROR_LIST_H */
