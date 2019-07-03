#ifndef SDL_PARSER_STRUCTURE_H
#define SDL_PARSER_STRUCTURE_H

#include <sdl_token_info.h>
#include <sdl_visibility.h>

#include <vector>
#include <fstream>

#define SDL_INFO_OUT(param, data)	if (output != nullptr) { (output)->param = (data); }
#define SDL_ERR_OUT(data)			SDL_INFO_OUT(err, (data)); 
#define SDL_FAIL()					SDL_INFO_OUT(failed, true);
#define SDL_FAILED(output)			(((output) != nullptr) ? (output)->failed : false)

namespace manta {

	class SdlValue;
	class SdlCompilationUnit;
	class SdlCompilationError;
	class SdlNode;
	class SdlContextTree;
	class Node;

	class SdlParserStructure {
	public:
		struct SdlReferenceQuery {
			SdlReferenceQuery();
			~SdlReferenceQuery();

			// Inputs
			SdlContextTree *inputContext;
			bool recordErrors;
		};

		struct SdlReferenceInfo {
			SdlReferenceInfo();
			~SdlReferenceInfo();

			// Ouputs
			SdlContextTree *newContext;
			SdlCompilationError *err;
			bool failed;
		};

	public:
		SdlParserStructure();
		virtual ~SdlParserStructure();

		const SdlTokenInfo *getSummaryToken() const { return &m_summaryToken; }
		void registerToken(const SdlTokenInfo *tokenInfo);

		void registerComponent(SdlParserStructure *child);
		int getComponentCount() const { return (int)m_components.size(); }

		virtual void setParentScope(SdlParserStructure *parentScope) { m_parentScope = parentScope; }
		virtual void setLogicalParent(SdlParserStructure *parent) { m_logicalParent = parent; }
		virtual SdlParserStructure *resolveName(const std::string &name) const;
		virtual SdlParserStructure *resolveLocalName(const std::string &name) const;

		bool getDefinitionsResolved() const { return m_definitionsResolved; }
		bool isExpanded() const { return m_isExpanded; }
		bool isValidated() const { return m_validated; }
		virtual SdlParserStructure *getImmediateReference(const SdlReferenceQuery &query, SdlReferenceInfo *output = nullptr);
		SdlParserStructure *getReference(const SdlReferenceQuery &query, SdlReferenceInfo *output = nullptr);

		SdlParserStructure *getExpansion() const { return m_expansion; }

		virtual SdlValue *getAsValue() { return nullptr; }

		bool allowsExternalAccess() const;

		void setVisibility(SdlVisibility visibility) { m_visibility = visibility; }
		SdlVisibility getVisibility() const { return m_visibility; }

		void setDefaultVisibility(SdlVisibility visibility) { m_defaultVisibility = visibility; }
		SdlVisibility getDefaultVisibility() const { return m_defaultVisibility; }

		void setCheckReferences(bool check) { m_checkReferences = check; }
		bool getCheckReferences() const { return m_checkReferences; }

		virtual bool isInputPoint() const { return false; }
		virtual bool isExternalInput() const { return false; }

		void setParentUnit(SdlCompilationUnit *unit) { m_parentUnit = unit; }
		SdlCompilationUnit *getParentUnit() const;

		virtual SdlNode *getAsNode() { return nullptr; }

	public:
		// Compilation stages
		void expand();
		void resolveDefinitions();
		void checkReferences(SdlContextTree *inputContext = nullptr);
		void checkInstantiation(SdlContextTree *inputContext = nullptr);
		void validate();

	protected:
		virtual void _expand();
		virtual void _resolveDefinitions();
		virtual void _validate();
		virtual void _checkInstantiation(SdlContextTree *inputContext);

	protected:
		SdlParserStructure *m_parentScope;
		SdlParserStructure *m_logicalParent;
		SdlTokenInfo m_summaryToken;

		SdlParserStructure *m_expansion;

		std::vector<SdlParserStructure *> m_components;

		SdlCompilationUnit *m_parentUnit;

	protected:
		// Visibility
		SdlVisibility m_defaultVisibility;
		SdlVisibility m_visibility;

		// Compilation flags
		bool m_isExpanded;
		bool m_definitionsResolved;
		bool m_validated;

		bool m_checkReferences;

	public:
		// Debugging
		void writeReferencesToFile(std::ofstream &file, SdlContextTree *context, int tabLevel = 0);
	};

} /* namespace manta */

#endif /* SDL_PARSER_STRUCTURE_H */
