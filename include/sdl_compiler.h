#ifndef SDL_COMPILER_H
#define SDL_COMPILER_H

#include <path.h>

#include <vector>
#include <string>

namespace manta {

	class SdlCompilationUnit;

	typedef Path SdlPath;

	class SdlCompiler {
	public:
		SdlCompiler();
		~SdlCompiler();

		SdlCompilationUnit *build(const SdlPath &scriptPath);
		SdlCompilationUnit *getUnit(const SdlPath &scriptPath) const;

		int getUnitCount() const { return (int)m_units.size(); }

	protected:
		bool isPathEquivalent(const SdlPath &a, const SdlPath &b) const;

		static bool hasEnding(std::string const &fullString, std::string const &ending);

	protected:
		std::vector<SdlCompilationUnit *> m_units;
	};

} /* namespace manta */

#endif /* SDL_COMPILER_H */
