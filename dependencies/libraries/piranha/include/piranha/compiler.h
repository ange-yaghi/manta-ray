#ifndef PIRANHA_IR_COMPILER_H
#define PIRANHA_IR_COMPILER_H

#include "path.h"
#include "ir_error_list.h"

#include <vector>
#include <string>

namespace piranha {

	class IrCompilationUnit;

	typedef Path IrPath;

	class IrCompiler {
	public:
		IrCompiler();
		~IrCompiler();

		IrCompilationUnit *compile(const IrPath &scriptPath);
		IrCompilationUnit *getUnit(const IrPath &scriptPath) const;

		int getUnitCount() const { return (int)m_units.size(); }

		const IrErrorList *getErrorList() const { return &m_errorList; }

		void addSearchPath(const IrPath &path);
		int getSearchPathCount() const { return (int)m_searchPaths.size(); }

		bool resolvePath(const IrPath &path, IrPath *target) const;

	protected:
		IrCompilationUnit *analyze(const IrPath &scriptPath);
		bool isPathEquivalent(const IrPath &a, const IrPath &b) const;

		static bool hasEnding(std::string const &fullString, std::string const &ending);

	protected:
		// Build steps
		void expand();
		void resolve();
		void validate();

	protected:
		IrErrorList m_errorList;
		std::vector<IrCompilationUnit *> m_units;

		std::vector<IrPath> m_searchPaths;
	};

} /* namespace piranha */

#endif /* PIRANHA_IR_COMPILER_H */
