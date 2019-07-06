#include <sdl_compiler.h>

#include <sdl_compilation_unit.h>
#include <sdl_import_statement.h>
#include <sdl_compilation_error.h>

manta::SdlCompiler::SdlCompiler() {
	/* void */
}

manta::SdlCompiler::~SdlCompiler() {
	/* void */
}

manta::SdlCompilationUnit *manta::SdlCompiler::analyze(const SdlPath &scriptPath) {
	SdlCompilationUnit *newUnit = getUnit(scriptPath);
	Path rootDir;
	scriptPath.getParentPath(&rootDir);

	if (newUnit == nullptr) {
		newUnit = new SdlCompilationUnit();
		newUnit->setErrorList(&m_errorList);
		SdlCompilationUnit::ParseResult parseResult = newUnit->parseFile(scriptPath);

		if (parseResult == SdlCompilationUnit::IO_ERROR) {
			delete newUnit;
			return nullptr;
		}

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

			if (!fullImportPath.exists()) {
				Path resolvedPath;
				bool fileFound = resolvePath(importPath, &resolvedPath);

				if (!fileFound) {
					newUnit->addCompilationError(new SdlCompilationError(*s->getSummaryToken(),
						SdlErrorCode::FileOpenFailed));
					continue;
				}
				else fullImportPath = resolvedPath;
			}

			// Recursively build
			SdlCompilationUnit *importUnit = analyze(fullImportPath);
			s->setUnit(importUnit);
			if (importUnit == nullptr) {
				newUnit->addCompilationError(new SdlCompilationError(*s->getSummaryToken(),
					SdlErrorCode::FileOpenFailed));
			}
			else newUnit->addDependency(importUnit);
		}
	}

	return newUnit;
}

manta::SdlCompilationUnit *manta::SdlCompiler::compile(const SdlPath &scriptPath) {
	SdlCompilationUnit *topLevel = analyze(scriptPath);

	// Expansion step
	expand();

	// Resolution step
	resolve();

	// Validation step
	validate();

	return topLevel;
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

void manta::SdlCompiler::addSearchPath(const SdlPath &path) {
	m_searchPaths.push_back(path);
}

bool manta::SdlCompiler::resolvePath(const SdlPath &path, SdlPath *target) const {
	if (path.exists()) {
		*target = path;
		return true;
	}

	int searchPathCount = getSearchPathCount();
	for (int i = 0; i < searchPathCount; i++) {
		SdlPath total = m_searchPaths[i].append(path);

		if (total.exists()) {
			*target = total;
			return true;
		}
	}

	return false;
}

bool manta::SdlCompiler::isPathEquivalent(const SdlPath &a, const SdlPath &b) const {
	return (a == b);
}

bool manta::SdlCompiler::hasEnding(std::string const &fullString, std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else return false;
}

void manta::SdlCompiler::expand() {
	int unitCount = getUnitCount();
	for (int i = 0; i < unitCount; i++) {
		SdlCompilationUnit *unit = m_units[i];
		unit->expand();
	}
}

void manta::SdlCompiler::resolve() {
	int unitCount = getUnitCount();
	for (int i = 0; i < unitCount; i++) {
		SdlCompilationUnit *unit = m_units[i];
		unit->resolveDefinitions();
		unit->checkInstantiation();
	}
}

void manta::SdlCompiler::validate() {
	int unitCount = getUnitCount();
	for (int i = 0; i < unitCount; i++) {
		SdlCompilationUnit *unit = m_units[i];
		unit->validate();
	}
}
