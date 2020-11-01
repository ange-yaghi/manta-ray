#ifndef PIRANHA_IR_ATTRIBUTE_DEFINITION_LIST_H
#define PIRANHA_IR_ATTRIBUTE_DEFINITION_LIST_H

#include "ir_parser_structure.h"

#include "ir_attribute_definition.h"

#include <vector>
#include <string>

namespace piranha {

    class IrAttributeDefinitionList : public IrParserStructure {
    public:
        IrAttributeDefinitionList();
        ~IrAttributeDefinitionList();

        void addDefinition(IrAttributeDefinition *definition);
        IrAttributeDefinition *getDefinition(int index) const { return m_definitions[index]; }
        int getDefinitionCount() const { return (int)m_definitions.size(); }

        IrAttributeDefinition *getDefinition(int index, bool isInput) const;
        IrAttributeDefinition *getInputDefinition(int index) const;

        IrAttributeDefinition *getOutputDefinition(int index) const;
        IrAttributeDefinition *getOutputDefinition(const std::string &name) const;
        IrAttributeDefinition *getAliasOutput() const;

        int getCount(IrAttributeDefinition::Direction direction) const;
        int getInputCount() const;
        int getOutputCount() const;

    protected:
        std::vector<IrAttributeDefinition *> m_definitions;
    };

} /* namespace piranha */

#endif /* PIRANHA_IR_ATTRIBUTE_DEFINITION_LIST_H */
