#ifndef PIRANHA_IR_STRUCTURE_LIST_H
#define PIRANHA_IR_STRUCTURE_LIST_H

#include "ir_parser_structure.h"

#include <vector>

namespace piranha {

    template <typename T>
    class IrStructureList : public IrParserStructure {
    public:
        IrStructureList() { /* void */ }
        ~IrStructureList() { /* void */ }

        void add(T *item) { m_items.push_back(item); registerComponent(item); }
        T *getItem(int index) const { return m_items[index]; }
        int getItemCount() const { return (int)m_items.size(); }

    protected:
        std::vector<T *> m_items;
    };

    class IrNode;

    typedef IrStructureList<IrNode> IrNodeList;

} /* namespace piranha */

#endif /* PIRANHA_IR_STRUCTURE_LIST_H */
