#ifndef SDL_NODE_H
#define SDL_NODE_H

#include <sdl_parser_structure.h>

#include <sdl_token_info.h>

#include <vector>
#include <fstream>

namespace manta {

	class SdlAttributeList;
	class SdlNodeDefinition;
	class SdlCompilationUnit;
	class SdlAttribute;
	class SdlAttributeDefinition;
	class Node;
	class SdlContextTree;

	class SdlNode : public SdlParserStructure {
	public:
		SdlNode();
		SdlNode(const SdlTokenInfo_string &type, const SdlTokenInfo_string &name, SdlAttributeList *attributes, 
			const SdlTokenInfo_string &library);
		SdlNode(const SdlTokenInfo_string &type, SdlAttributeList *attributes, 
			const SdlTokenInfo_string &library);
		~SdlNode();

		SdlTokenInfo getTypeToken() const { return m_type; }
		SdlTokenInfo getNameToken() const { return m_name; }

		const std::string &getType() const { return m_type.data; }
		const std::string &getName() const { return m_name.data; }

		void setAttributes(SdlAttributeList *list);
		SdlAttributeList *getAttributes() const { return m_attributes; }
		SdlAttribute *getAttribute(const std::string &name, int *count = nullptr) const;

		virtual void setParentScope(SdlParserStructure *parentScope);

		virtual SdlValue *getDefaultOutputValue();

		virtual SdlNode *getAsNode() { return this; }

		void writeTraceToFile(std::ofstream &file);

		virtual void checkReferences(SdlContextTree *inputContext = nullptr);

	protected:
		SdlTokenInfo_string m_type;
		SdlTokenInfo_string m_name;
		SdlTokenInfo_string m_library;

		SdlAttributeList *m_attributes;

		SdlAttribute *getAttribute(SdlAttributeDefinition *definition, int *count = nullptr) const;

		// Resolution stage
	public:
		SdlNodeDefinition *getDefinition() const { return m_definition; }
		void setDefinition(SdlNodeDefinition *definition) { m_definition = definition; }

		virtual SdlParserStructure *resolveLocalName(const std::string &name) const;

	protected:
		virtual void _resolveDefinitions();
		virtual void _validate();
		virtual void _checkInstantiation();

		void resolveNodeDefinition();
		void resolveAttributeDefinitions();

		SdlNodeDefinition *m_definition;

	public:
		Node *generateNode(SdlContextTree *context = nullptr);

	protected:
		struct NodeTableEntry {
			Node *generatedNode;
			SdlContextTree *context;
		};

		NodeTableEntry *getTableEntry(SdlContextTree *context);
		NodeTableEntry *newTableEntry(SdlContextTree *context);

		std::vector<NodeTableEntry> m_nodeTable;
	};

} /* namespace manta */

#endif /* SDL_NODE_H */
