#ifndef PIRANHA_IR_ERROR_LIST_H
#define PIRANHA_IR_ERROR_LIST_H

#include <vector>

namespace piranha {

	class IrCompilationError;

	class IrErrorList {
	public:
		IrErrorList();
		~IrErrorList();

		void addCompilationError(IrCompilationError *err);
		IrCompilationError *getCompilationError(int index) const { return m_compilationErrors[index]; }
		int getErrorCount() const { return (int)m_compilationErrors.size(); }

		void destroy();

	protected:
		std::vector<IrCompilationError *> m_compilationErrors;
	};

} /* namespace piranha */

#endif /* PIRANHA_IR_ERROR_LIST_H */
