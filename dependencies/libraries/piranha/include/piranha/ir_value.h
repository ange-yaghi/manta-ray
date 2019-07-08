#ifndef PIRANHA_IR_VALUE_H
#define PIRANHA_IR_VALUE_H

#include "ir_parser_structure.h"

#include <vector>

namespace piranha {

	class IrAttribute;
	class NodeOutput;
	class Node;
	class NodeProgram;
	class IrContextTree;

	class IrValue : public IrParserStructure {
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
			IrContextTree *context;
		};

	public:
		IrValue(VALUE_TYPE type);
		virtual ~IrValue();

		VALUE_TYPE getType() const { return m_type; }

		virtual IrValue *getAsValue() { return this; }

		virtual bool isGeneric() const { return false; }

	private:
		VALUE_TYPE m_type;

	public:
		NodeOutput *generateNodeOutput(IrContextTree *context, NodeProgram *program);
		Node *generateNode(IrContextTree *context, NodeProgram *program);

	protected:
		virtual NodeOutput *_generateNodeOutput(IrContextTree *context, NodeProgram *program);
		virtual Node *_generateNode(IrContextTree *context, NodeProgram *program);

		GenerationTableEntry *getEntry(IrContextTree *context);
		GenerationTableEntry *newEntry(IrContextTree *context);

		std::vector<GenerationTableEntry> m_generationTable;
	};

} /* namespace piranha */

#endif /* PIRANHA_IR_VALUE_H */
