#ifndef SDL_VALUE_H
#define SDL_VALUE_H

#include <sdl_parser_structure.h>

#include <vector>

namespace manta {

	class SdlAttribute;
	class NodeOutput;
	class Node;
	class NodeProgram;
	class SdlContextTree;

	class SdlValue : public SdlParserStructure {
	public:
		enum VALUE_TYPE {
			CONSTANT_FLOAT,
			CONSTANT_INT,
			CONSTANT_STRING,
			CONSTANT_LABEL,
			CONSTANT_BOOL,

			BINARY_OPERATION,
			UNARY_OPERATION,

			NODE_REF,
			GENERIC
		};

	public:
		struct GenerationTableEntry {
			NodeOutput *nodeGeneratedOutput;
			Node *nodeReference;
			SdlContextTree *context;
		};

	public:
		SdlValue(VALUE_TYPE type);
		virtual ~SdlValue();

		VALUE_TYPE getType() const { return m_type; }

		virtual SdlValue *getAsValue() { return this; }

		virtual bool isGeneric() const { return false; }

	private:
		VALUE_TYPE m_type;

	public:
		NodeOutput *generateNodeOutput(SdlContextTree *context, NodeProgram *program);
		Node *generateNode(SdlContextTree *context, NodeProgram *program);

	protected:
		virtual NodeOutput *_generateNodeOutput(SdlContextTree *context, NodeProgram *program);
		virtual Node *_generateNode(SdlContextTree *context, NodeProgram *program);

		GenerationTableEntry *getEntry(SdlContextTree *context);
		GenerationTableEntry *newEntry(SdlContextTree *context);

		std::vector<GenerationTableEntry> m_generationTable;
	};

} /* namespace manta */

#endif /* SDL_VALUE_H */
