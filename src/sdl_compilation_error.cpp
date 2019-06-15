#include <sdl_compilation_error.h>

manta::SdlCompilationError::SdlCompilationError(
		const SdlTokenInfo &location, const SdlErrorCode_struct &code) {
	m_errorLocation = location;
	m_code = code;
}

manta::SdlCompilationError::~SdlCompilationError() {
	/* void */
}

// Error definitions --------------------------------------

// Helper macro
#define ERR(tag) const manta::SdlErrorCode_struct manta::SdlErrorCode::tag

// [IO] - IO Errors
ERR(FileOpenFailed) =			{ "IO", "0001", "Could not open file" };

// [S] - Scanning errors
ERR(UnidentifiedToken) =		{ "S", "0001", "Unidentified token" };

// [P] - Parsing errors
ERR(UnexpectedToken) =			{ "P", "0001", "Unexpected token" };

// [R] - Resolution errors
ERR(UndefinedNodeType) =		{ "R", "0010", "Undefined node type" };
ERR(ArgumentPositionOutOfBounds) =
								{ "R", "0020", "Argument position out of bounds" };
ERR(PortNotFound) =				{ "R", "0030", "Port not found" };
ERR(UsingOutputPortAsInput) =	{ "R", "0040", "Using output port as input" };
ERR(UnresolvedReference) =		{ "R", "0050", "Unresolved reference" };
ERR(UndefinedMember) =			{ "R", "0060", "Undefined member" };
ERR(AccessingInternalMember) =	{ "R", "0061", "Invalid reference to an internal member" };
ERR(InvalidRightHandOperand) =	{ "R", "0070", "Invalid right-hand operand" };

// [V] - Validation errors
ERR(InputSpecifiedMultipleTimes) = 
								{ "V", "0001", "Input specified multiple times" };
