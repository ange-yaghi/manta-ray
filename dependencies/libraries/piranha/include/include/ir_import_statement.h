#ifndef PIRANHA_IR_IMPORT_STATEMENT_H
#define PIRANHA_IR_IMPORT_STATEMENT_H

#include <string>

#include "ir_parser_structure.h"
#include "ir_token_info.h"
#include "ir_visibility.h"

namespace piranha {

    class IrCompilationUnit;

    class IrImportStatement : public IrParserStructure {
    public:
        IrImportStatement(const IrTokenInfo_string &libName);
        ~IrImportStatement();

        std::string getLibName() const { return m_libName.data; }

        void setUnit(IrCompilationUnit *unit) { m_unit = unit; }
        IrCompilationUnit *getUnit() const { return m_unit; }

        void setShortName(const IrTokenInfo_string &shortName) { m_shortName = shortName; }
        bool hasShortName() const { return m_shortName.specified; }
        std::string getShortName() const { return m_shortName.data; }

    protected:
        IrTokenInfo_string m_shortName;
        IrTokenInfo_string m_libName;
        IrCompilationUnit *m_unit;
    };

} /* namespace piranha */

#endif /* PIRANHA_IR_IMPORT_STATEMENT */
