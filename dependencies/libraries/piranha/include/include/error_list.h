#ifndef PIRANHA_IR_ERROR_LIST_H
#define PIRANHA_IR_ERROR_LIST_H

#include <vector>

namespace piranha {

    class CompilationError;

    class ErrorList {
    public:
        ErrorList();
        ~ErrorList();

        void addCompilationError(CompilationError *err);
        CompilationError *getCompilationError(int index) const { return m_compilationErrors[index]; }
        int getErrorCount() const { return (int)m_compilationErrors.size(); }

        void free();

    protected:
        std::vector<CompilationError *> m_compilationErrors;
    };

} /* namespace piranha */

#endif /* PIRANHA_IR_ERROR_LIST_H */
