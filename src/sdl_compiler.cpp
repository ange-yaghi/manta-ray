#include <sdl_compiler.h>

#include <sdl_compilation_unit.h>
#include <sdl_import_statement.h>

manta::SdlCompiler::SdlCompiler() {
	/* void */
}

manta::SdlCompiler::~SdlCompiler() {
	/* void */
}

manta::SdlCompilationUnit *manta::SdlCompiler::build(const SdlPath &scriptPath) {
	SdlCompilationUnit *newUnit = getUnit(scriptPath);
	Path rootDir;
	scriptPath.getParentPath(&rootDir);

	if (newUnit == nullptr) {
		newUnit = new SdlCompilationUnit();
		newUnit->parseFile(scriptPath);

		m_units.push_back(newUnit);

		int importCount = newUnit->getImportStatementCount();
		for (int i = 0; i < importCount; i++) {
			SdlImportStatement *s = newUnit->getImportStatement(i);
			std::string libName = s->getLibName();

			if (!hasEnding(libName, ".mr")) {
				libName += ".mr";
			}

			Path importPath(libName);
			Path fullImportPath;

			if (importPath.isAbsolute()) {
				// TODO: Warn about use of absolute path
				fullImportPath = importPath;
			}
			else {
				fullImportPath = rootDir.append(importPath);
			}

			// Recursively build
			SdlCompilationUnit *importUnit = build(fullImportPath);
			newUnit->addDependency(importUnit);
		}
	}

	return newUnit;
}

manta::SdlCompilationUnit *manta::SdlCompiler::getUnit(const SdlPath &scriptPath) const {
	int nUnits = (int)m_units.size();

	for (int i = 0; i < nUnits; i++) {
		SdlCompilationUnit *unit = m_units[i];
		if (unit->getPath() == scriptPath) {
			return unit;
		}
	}

	return nullptr;
}

bool manta::SdlCompiler::isPathEquivalent(const SdlPath &a, const SdlPath &b) const {
	return (a == b);
}

bool manta::SdlCompiler::hasEnding(std::string const &fullString, std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}
