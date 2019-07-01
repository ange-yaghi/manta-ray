#ifndef SDL_COMPILATION_ERROR_H
#define SDL_COMPILATION_ERROR_H

#include <sdl_token_info.h>

#include <string>

namespace manta {

	class SdlCompilationUnit;
	class SdlContextTree;

	struct SdlErrorCode_struct {
		std::string stage;
		std::string code;
		std::string info;
	};

	namespace SdlErrorCode {
		// List of all error codes

		// [IO] - IO errors
		extern const SdlErrorCode_struct FileOpenFailed;

		// [S] - Scanning errors
		extern const SdlErrorCode_struct UnidentifiedToken;

		// [P] - Parsing errors
		extern const SdlErrorCode_struct UnexpectedToken;

		// [R] - Resolution Errors
		extern const SdlErrorCode_struct UndefinedNodeType;
		extern const SdlErrorCode_struct ArgumentPositionOutOfBounds;
		extern const SdlErrorCode_struct PortNotFound;
		extern const SdlErrorCode_struct UsingOutputPortAsInput;
		extern const SdlErrorCode_struct UnresolvedReference;
		extern const SdlErrorCode_struct UndefinedMember;
		extern const SdlErrorCode_struct AccessingInternalMember;
		extern const SdlErrorCode_struct CannotFindDefaultValue;

		// [V] - Validation errors
		extern const SdlErrorCode_struct InputSpecifiedMultipleTimes;
		extern const SdlErrorCode_struct SymbolUsedMultipleTimes;
		extern const SdlErrorCode_struct InputNotConnected;
		extern const SdlErrorCode_struct OutputWithNoDefinition;
		extern const SdlErrorCode_struct BuiltinOutputWithDefinition;
		extern const SdlErrorCode_struct BuiltinOutputMissingType;
		extern const SdlErrorCode_struct StandardOutputWithType;
		extern const SdlErrorCode_struct InputSpecifiedMultipleTimesPositional;
		extern const SdlErrorCode_struct DuplicateNodeDefinition;
	};

	class SdlCompilationError {
	public:
		SdlCompilationError(const SdlTokenInfo &location, const SdlErrorCode_struct &code, 
			SdlContextTree *instantiation = nullptr);
		~SdlCompilationError();

		const SdlTokenInfo *getErrorLocation() const { return &m_errorLocation; }
		void setErrorLocation(SdlTokenInfo &loc) { m_errorLocation = loc; }

		void setErrorCode(const SdlErrorCode_struct &code) { m_code = code; }
		const SdlErrorCode_struct &getErrorCode() const { return m_code; }

		void setCompilationUnit(SdlCompilationUnit *unit) { m_unit = unit; }
		SdlCompilationUnit *getCompilationUnit() const { return m_unit; }

		bool isInstantiationError() const { return m_instantiation != nullptr; }
		SdlContextTree *getInstantiation() const { return m_instantiation; }

	protected:
		SdlTokenInfo m_errorLocation;
		SdlErrorCode_struct m_code;
		SdlCompilationUnit *m_unit;

		SdlContextTree *m_instantiation;
	};

} /* namespace manta */

#endif /* SDL_COMPILATION_ERROR_H */
