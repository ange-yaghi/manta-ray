#ifndef SDL_COMPILER_H
#define SDL_COMPILER_H

#include <vector>
#include <string>

namespace manta {

	class SdlCompilationUnit;

	typedef std::string SdlPath;

	class SdlCompiler {
	public:
		SdlCompiler();
		~SdlCompiler();

		SdlCompilationUnit *build(const SdlPath &scriptPath);
		void addImportDirectory(const SdlPath &path) { m_importDirectories.push_back(path); }

		SdlCompilationUnit *getUnit(const std::string &scriptPath) const;

		SdlPath resolveModuleName(const SdlPath &scriptPath) const;

	protected:
		bool isPathEquivalent(const SdlPath &a, const SdlPath &b) const;

	protected:
		std::vector<SdlPath> m_importDirectories;
		std::vector<SdlCompilationUnit *> m_units;

		std::string m_mainPath;
	};

} /* namespace manta */

#endif /* SDL_COMPILER_H */
