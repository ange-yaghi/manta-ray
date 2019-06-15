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
ERR(UndefinedNodeType) =		{ "R", "0001", "Undefined node type" };
ERR(ArgumentPositionOutOfBounds) =
								{ "R", "0002", "Argument position out of bounds" };
ERR(PortNotFound) =				{ "R", "0003", "Port not found" };
ERR(UsingOutputPortAsInput) =	{ "R", "0004", "Using output port as input" };
ERR(UnresolvedReference) =		{ "R", "0005", "Unresolved reference" };
ERR(UndefinedMember) =			{ "R", "0006", "Undefined member" };
ERR(InvalidRightHandOperand) =	{ "R", "0007", "Invalid right-hand operand" };
