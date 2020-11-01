#ifndef PIRANHA_IR_ATTRIBUTE_LIST_H
#define PIRANHA_IR_ATTRIBUTE_LIST_H

#include "ir_parser_structure.h"

#include <vector>

namespace piranha {

    class IrAttribute;
    class IrAttributeDefinition;

    class IrAttributeList : public IrParserStructure {
    public:
        IrAttributeList();
        ~IrAttributeList();

        void addAttribute(IrAttribute *attribute);
        IrAttribute *getAttribute(int index) const { return m_attributes[index]; }
        IrAttribute *getAttribute(IrAttributeDefinition *definition) const;
        int getAttributeCount() const { return (int)m_attributes.size(); }

        virtual void free();

    protected:
        std::vector<IrAttribute *> m_attributes;
    };

} /* namespace piranha */

#endif /* PIRANHA_IR_ATTRIBUTE_LIST_H */
