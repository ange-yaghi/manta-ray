#include <sdl_compilation_error.h>

manta::SdlCompilationError::SdlCompilationError(const SdlTokenInfo &location, 
			const SdlErrorCode_struct &code, SdlParserStructure *instantiation) {
	m_errorLocation = location;
	m_code = code;
	m_instantiation = instantiation;
}

manta::SdlCompilationError::~SdlCompilationError() {
	/* void */
}

// Error definitions --------------------------------------

// Helper macro
#define ERR(tag) const manta::SdlErrorCode_struct manta::SdlErrorCode::tag

// [IO] - IO Errors
ERR(FileOpenFailed) =			{ "IO", "0010", "Could not open file" };

// [S] - Scanning errors
ERR(UnidentifiedToken) =		{ "S", "0010", "Unidentified token" };

// [P] - Parsing errors
ERR(UnexpectedToken) =			{ "P", "0010", "Unexpected token" };

// [R] - Resolution errors
ERR(UndefinedNodeType) =		{ "R", "0010", "Undefined node type" };
ERR(ArgumentPositionOutOfBounds) =
								{ "R", "0020", "Argument position out of bounds" };
ERR(PortNotFound) =				{ "R", "0030", "Port not found" };
ERR(UsingOutputPortAsInput) =	{ "R", "0040", "Using output port as input" };
ERR(UnresolvedReference) =		{ "R", "0050", "Unresolved reference" };
ERR(UndefinedMember) =			{ "R", "0060", "Undefined member" };
ERR(AccessingInternalMember) =	{ "R", "0061", "Invalid reference to an internal member" };
ERR(CannotFindDefaultValue) =	{ "R", "0070", "Cannot find default value" };

// [V] - Validation errors
ERR(InputSpecifiedMultipleTimes) = 
								{ "V", "0010", "Input specified multiple times" };
ERR(SymbolUsedMultipleTimes) =	{ "V", "0030", "Symbol used multiple times" };
ERR(InputNotConnected) =		{ "V", "0040", "Input not connected" };
ERR(OutputWithNoDefinition) =	{ "V", "0050", "Output with no definition" };
ERR(BuiltinOutputWithDefinition) =
								{ "V", "0051", "Built-in output given a definition" };
ERR(BuiltinOutputMissingType) = { "V", "0052", "Built-in output missing a definition" };
ERR(StandardOutputWithType) =	{ "V", "0051", "Built-in output given a definition" };
ERR(InputSpecifiedMultipleTimesPositional) =
								{ "V", "0011", "Input specified multiple times by positional" };
ERR(DuplicateNodeDefinition) =	{ "V", "0060", "Multiple definitions with the same name" };
