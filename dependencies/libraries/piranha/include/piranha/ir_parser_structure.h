#ifndef PIRANHA_IR_PARSER_STRUCTURE_H
#define PIRANHA_IR_PARSER_STRUCTURE_H

#include "ir_token_info.h"
#include "ir_visibility.h"

#include <vector>
#include <fstream>

#define IR_RESET(query)			if (output != nullptr) { output->reset(); output->newContext = (query).inputContext; }
#define IR_INFO_OUT(param, data)	if (output != nullptr) { output->param = (data); }
#define IR_ERR_OUT(data)			IR_INFO_OUT(err, (data)); 
#define IR_FAIL()					IR_INFO_OUT(failed, true);
#define IR_DEAD_END()				IR_INFO_OUT(reachedDeadEnd, true);
#define IR_FAILED(output)			(((output) != nullptr) ? (output)->failed : false)
#define IR_EMPTY_CONTEXT()			(query.inputContext == nullptr || query.inputContext->getContext() == nullptr)

namespace piranha {

	class IrValue;
	class IrCompilationUnit;
	class IrCompilationError;
	class IrNode;
	class IrContextTree;
	class Node;

	class IrParserStructure {
	public:
		struct IrReferenceQuery {
			IrReferenceQuery();
			~IrReferenceQuery();

			// Inputs
			IrContextTree *inputContext;
			bool recordErrors;
		};

		struct IrReferenceInfo {
			IrReferenceInfo();
			~IrReferenceInfo();

			void reset();

			// Ouputs
			IrContextTree *newContext;
			IrCompilationError *err;

			bool failed;
			bool reachedDeadEnd;
			bool touchedMainContext;
		};

	public:
		IrParserStructure();
		virtual ~IrParserStructure();

		const IrTokenInfo *getSummaryToken() const { return &m_summaryToken; }
		void registerToken(const IrTokenInfo *tokenInfo);

		void registerComponent(IrParserStructure *child);
		int getComponentCount() const { return (int)m_components.size(); }

		virtual void setParentScope(IrParserStructure *parentScope) { m_parentScope = parentScope; }
		virtual void setLogicalParent(IrParserStructure *parent) { m_logicalParent = parent; }
		virtual IrParserStructure *resolveName(const std::string &name) const;
		virtual IrParserStructure *resolveLocalName(const std::string &name) const;

		bool getDefinitionsResolved() const { return m_definitionsResolved; }
		bool isExpanded() const { return m_isExpanded; }
		bool isValidated() const { return m_validated; }
		virtual IrParserStructure *getImmediateReference(const IrReferenceQuery &query, IrReferenceInfo *output = nullptr);
		IrParserStructure *getReference(const IrReferenceQuery &query, IrReferenceInfo *output = nullptr);

		IrParserStructure *getExpansion() const { return m_expansion; }

		virtual IrValue *getAsValue() { return nullptr; }

		bool allowsExternalAccess() const;

		void setVisibility(IrVisibility visibility) { m_visibility = visibility; }
		IrVisibility getVisibility() const { return m_visibility; }

		void setDefaultVisibility(IrVisibility visibility) { m_defaultVisibility = visibility; }
		IrVisibility getDefaultVisibility() const { return m_defaultVisibility; }

		void setCheckReferences(bool check) { m_checkReferences = check; }
		bool getCheckReferences() const { return m_checkReferences; }

		virtual bool isInputPoint() const { return false; }
		virtual bool isExternalInput() const { return false; }

		void setParentUnit(IrCompilationUnit *unit) { m_parentUnit = unit; }
		IrCompilationUnit *getParentUnit() const;

		virtual IrNode *getAsNode() { return nullptr; }

	public:
		// Compilation stages
		void expand();
		void resolveDefinitions();
		virtual void checkReferences(IrContextTree *inputContext = nullptr);
		void checkInstantiation();
		void validate();

	protected:
		virtual void _expand();
		virtual void _resolveDefinitions();
		virtual void _validate();
		virtual void _checkInstantiation();

	protected:
		IrParserStructure *m_parentScope;
		IrParserStructure *m_logicalParent;
		IrTokenInfo m_summaryToken;

		IrParserStructure *m_expansion;

		std::vector<IrParserStructure *> m_components;

		IrCompilationUnit *m_parentUnit;

	protected:
		// Visibility
		IrVisibility m_defaultVisibility;
		IrVisibility m_visibility;

		// Compilation flags
		bool m_isExpanded;
		bool m_definitionsResolved;
		bool m_validated;

		bool m_checkReferences;

	public:
		// Debugging
		void writeReferencesToFile(std::ofstream &file, IrContextTree *context, int tabLevel = 0);
	};

} /* namespace piranha */

#endif /* PIRANHA_IR_PARSER_STRUCTURE_H */
