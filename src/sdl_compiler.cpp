#include <sdl_compiler.h>

#include <sdl_compilation_unit.h>

manta::SdlCompiler::SdlCompiler() {
	/* void */
}

manta::SdlCompiler::~SdlCompiler() {
	/* void */
}

manta::SdlCompilationUnit *manta::SdlCompiler::build(const SdlPath &scriptPath) {
	SdlCompilationUnit *newUnit = getUnit(scriptPath);

	if (newUnit == nullptr) {
		newUnit = new SdlCompilationUnit();
		newUnit->parseFile("", scriptPath);

		m_units.push_back(newUnit);
	}

	return newUnit;
}

manta::SdlCompilationUnit *manta::SdlCompiler::getUnit(const SdlPath &scriptPath) const {
	int nUnits = (int)m_units.size();

	for (int i = 0; i < nUnits; i++) {
		SdlCompilationUnit *unit = m_units[i];
		//if (unit->getFullPath() == scriptPath) {
		//	return unit;
		//}
	}

	return nullptr;
}

bool manta::SdlCompiler::isPathEquivalent(const SdlPath &a, const SdlPath &b) const {
	return (a == b);
}
