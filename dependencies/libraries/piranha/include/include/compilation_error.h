#ifndef PIRANHA_COMPILATION_ERROR_H
#define PIRANHA_COMPILATION_ERROR_H

#include "ir_token_info.h"

#include <string>

namespace piranha {

    class IrCompilationUnit;
    class IrContextTree;

    struct ErrorCode_struct {
        std::string stage;
        std::string code;
        std::string info;
    };

    namespace ErrorCode {
        // List of all error codes

        // [IO] - IO errors
        extern const ErrorCode_struct FileOpenFailed;

        // [S] - Scanning errors
        extern const ErrorCode_struct UnidentifiedToken;

        // [P] - Parsing errors
        extern const ErrorCode_struct UnexpectedToken;

        // [E] - Expansion errors
        extern const ErrorCode_struct InvalidOperandTypes;
        extern const ErrorCode_struct CircularDefinition;

        // [R] - Resolution Errors
        extern const ErrorCode_struct UndefinedNodeType;
        extern const ErrorCode_struct ArgumentPositionOutOfBounds;
        extern const ErrorCode_struct PortNotFound;
        extern const ErrorCode_struct UsingOutputPortAsInput;
        extern const ErrorCode_struct UnresolvedReference;
        extern const ErrorCode_struct UndefinedMember;
        extern const ErrorCode_struct AccessingInternalMember;
        extern const ErrorCode_struct CannotFindDefaultValue;
        extern const ErrorCode_struct CircularReference;

        // [V] - Validation errors
        extern const ErrorCode_struct InputSpecifiedMultipleTimes;
        extern const ErrorCode_struct SymbolUsedMultipleTimes;
        extern const ErrorCode_struct InputNotConnected;
        extern const ErrorCode_struct OutputWithNoDefinition;
        extern const ErrorCode_struct BuiltinOutputWithDefinition;
        extern const ErrorCode_struct BuiltinOutputMissingType;
        extern const ErrorCode_struct InputSpecifiedMultipleTimesPositional;
        extern const ErrorCode_struct DuplicateNodeDefinition;
        extern const ErrorCode_struct UndefinedBuiltinType;
        extern const ErrorCode_struct UndefinedBuiltinInput;
        extern const ErrorCode_struct UndefinedBuiltinOutput;
        extern const ErrorCode_struct ModifyAttributeMismatch;
        extern const ErrorCode_struct ToggleAttributeMismatch;
        extern const ErrorCode_struct AliasAttributeMismatch;

        // [T] - Type errors
        extern const ErrorCode_struct IncompatibleType;
        extern const ErrorCode_struct IncompatibleDefaultType;
        extern const ErrorCode_struct IncompatibleOutputDefinitionType;
    };

    class CompilationError {
    public:
        CompilationError(const IrTokenInfo &location, const ErrorCode_struct &code, 
            IrContextTree *instantiation = nullptr);
        ~CompilationError();

        const IrTokenInfo *getErrorLocation() const { return &m_errorLocation; }
        void setErrorLocation(IrTokenInfo &loc) { m_errorLocation = loc; }

        void setErrorCode(const ErrorCode_struct &code) { m_code = code; }
        const ErrorCode_struct &getErrorCode() const { return m_code; }

        void setCompilationUnit(IrCompilationUnit *unit) { m_unit = unit; }
        IrCompilationUnit *getCompilationUnit() const { return m_unit; }

        bool isInstantiationError() const;
        IrContextTree *getInstantiation() const { return m_instantiation; }

    protected:
        IrTokenInfo m_errorLocation;
        ErrorCode_struct m_code;
        IrCompilationUnit *m_unit;

        IrContextTree *m_instantiation;
    };

} /* namespace piranha */

#endif /* PIRANHA_COMPILATION_ERROR_H */
