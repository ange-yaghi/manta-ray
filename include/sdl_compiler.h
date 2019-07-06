#ifndef SDL_COMPILER_H
#define SDL_COMPILER_H

#include <path.h>
#include <sdl_error_list.h>

#include <vector>
#include <string>

namespace manta {

	class SdlCompilationUnit;

	typedef Path SdlPath;

	class SdlCompiler {
	public:
		SdlCompiler();
		~SdlCompiler();

		SdlCompilationUnit *compile(const SdlPath &scriptPath);
		SdlCompilationUnit *getUnit(const SdlPath &scriptPath) const;

		int getUnitCount() const { return (int)m_units.size(); }

		const SdlErrorList *getErrorList() const { return &m_errorList; }

		void addSearchPath(const SdlPath &path);
		int getSearchPathCount() const { return (int)m_searchPaths.size(); }

		bool resolvePath(const SdlPath &path, SdlPath *target) const;

	protected:
		SdlCompilationUnit *analyze(const SdlPath &scriptPath);
		bool isPathEquivalent(const SdlPath &a, const SdlPath &b) const;

		static bool hasEnding(std::string const &fullString, std::string const &ending);

	protected:
		// Build steps
		void expand();
		void resolve();
		void validate();

	protected:
		SdlErrorList m_errorList;
		std::vector<SdlCompilationUnit *> m_units;

		std::vector<SdlPath> m_searchPaths;
	};

} /* namespace manta */

#endif /* SDL_COMPILER_H */
