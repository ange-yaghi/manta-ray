#ifndef PIRANHA_IR_NODE_H
#define PIRANHA_IR_NODE_H

#include "ir_parser_structure.h"

#include "ir_token_info.h"

#include <vector>
#include <fstream>

namespace piranha {

	class IrAttributeList;
	class IrNodeDefinition;
	class IrCompilationUnit;
	class IrAttribute;
	class IrAttributeDefinition;
	class IrContextTree;
	class Node;
	class NodeProgram;

	class IrNode : public IrParserStructure {
	public:
		IrNode();
		IrNode(const IrTokenInfo_string &type, const IrTokenInfo_string &name, IrAttributeList *attributes, 
			const IrTokenInfo_string &library);
		IrNode(const IrTokenInfo_string &type, IrAttributeList *attributes, 
			const IrTokenInfo_string &library);
		~IrNode();

		IrTokenInfo getTypeToken() const { return m_type; }
		IrTokenInfo getNameToken() const { return m_name; }

		const std::string &getType() const { return m_type.data; }
		const std::string &getName() const { return m_name.data; }

		void setAttributes(IrAttributeList *list);
		IrAttributeList *getAttributes() const { return m_attributes; }
		IrAttribute *getAttribute(const std::string &name, int *count = nullptr) const;

		virtual void setParentScope(IrParserStructure *parentScope);

		virtual IrValue *getDefaultOutputValue();

		virtual IrNode *getAsNode() { return this; }

		void writeTraceToFile(std::ofstream &file);

		virtual void checkReferences(IrContextTree *inputContext = nullptr);

	protected:
		IrTokenInfo_string m_type;
		IrTokenInfo_string m_name;
		IrTokenInfo_string m_library;

		IrAttributeList *m_attributes;

		IrAttribute *getAttribute(IrAttributeDefinition *definition, int *count = nullptr) const;

		// Resolution stage
	public:
		IrNodeDefinition *getDefinition() const { return m_definition; }
		void setDefinition(IrNodeDefinition *definition) { m_definition = definition; }

		virtual IrParserStructure *resolveLocalName(const std::string &name) const;

	protected:
		virtual void _resolveDefinitions();
		virtual void _validate();
		virtual void _checkInstantiation();

		void resolveNodeDefinition();
		void resolveAttributeDefinitions();

		IrNodeDefinition *m_definition;

	public:
		Node *generateNode(IrContextTree *context, NodeProgram *program);

	protected:
		struct NodeTableEntry {
			Node *generatedNode;
			IrContextTree *context;
		};

		NodeTableEntry *getTableEntry(IrContextTree *context);
		NodeTableEntry *newTableEntry(IrContextTree *context);

		std::vector<NodeTableEntry *> m_nodeTable;
	};

} /* namespace piranha */

#endif /* PIRANHA_IR_NODE_H */
