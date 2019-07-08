#ifndef PIRANHA_IR_COMPILATION_ERROR_H
#define PIRANHA_IR_COMPILATION_ERROR_H

#include "ir_token_info.h"

#include <string>

namespace piranha {

	class IrCompilationUnit;
	class IrContextTree;

	struct IrErrorCode_struct {
		std::string stage;
		std::string code;
		std::string info;
	};

	namespace IrErrorCode {
		// List of all error codes

		// [IO] - IO errors
		extern const IrErrorCode_struct FileOpenFailed;

		// [S] - Scanning errors
		extern const IrErrorCode_struct UnidentifiedToken;

		// [P] - Parsing errors
		extern const IrErrorCode_struct UnexpectedToken;

		// [R] - Resolution Errors
		extern const IrErrorCode_struct UndefinedNodeType;
		extern const IrErrorCode_struct ArgumentPositionOutOfBounds;
		extern const IrErrorCode_struct PortNotFound;
		extern const IrErrorCode_struct UsingOutputPortAsInput;
		extern const IrErrorCode_struct UnresolvedReference;
		extern const IrErrorCode_struct UndefinedMember;
		extern const IrErrorCode_struct AccessingInternalMember;
		extern const IrErrorCode_struct CannotFindDefaultValue;

		// [V] - Validation errors
		extern const IrErrorCode_struct InputSpecifiedMultipleTimes;
		extern const IrErrorCode_struct SymbolUsedMultipleTimes;
		extern const IrErrorCode_struct InputNotConnected;
		extern const IrErrorCode_struct OutputWithNoDefinition;
		extern const IrErrorCode_struct BuiltinOutputWithDefinition;
		extern const IrErrorCode_struct BuiltinOutputMissingType;
		extern const IrErrorCode_struct StandardOutputWithType;
		extern const IrErrorCode_struct InputSpecifiedMultipleTimesPositional;
		extern const IrErrorCode_struct DuplicateNodeDefinition;
	};

	class IrCompilationError {
	public:
		IrCompilationError(const IrTokenInfo &location, const IrErrorCode_struct &code, 
			IrContextTree *instantiation = nullptr);
		~IrCompilationError();

		const IrTokenInfo *getErrorLocation() const { return &m_errorLocation; }
		void setErrorLocation(IrTokenInfo &loc) { m_errorLocation = loc; }

		void setErrorCode(const IrErrorCode_struct &code) { m_code = code; }
		const IrErrorCode_struct &getErrorCode() const { return m_code; }

		void setCompilationUnit(IrCompilationUnit *unit) { m_unit = unit; }
		IrCompilationUnit *getCompilationUnit() const { return m_unit; }

		bool isInstantiationError() const;
		IrContextTree *getInstantiation() const { return m_instantiation; }

	protected:
		IrTokenInfo m_errorLocation;
		IrErrorCode_struct m_code;
		IrCompilationUnit *m_unit;

		IrContextTree *m_instantiation;
	};

} /* namespace piranha */

#endif /* PIRANHA_IR_COMPILATION_ERROR_H */
