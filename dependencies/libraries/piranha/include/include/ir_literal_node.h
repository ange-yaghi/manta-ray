#ifndef PIRANHA_IR_LITERAL_NODE_H
#define PIRANHA_IR_LITERAL_NODE_H

#include "ir_node.h"

#include "node.h"
#include "literal_node.h"
#include "fundamental_types.h"
#include "exceptions.h"

namespace piranha {

    template <typename NativeType>
    class IrLiteralNode : public IrNode {
    public:
        IrLiteralNode() {
            reset(m_literalData);
        }

        ~IrLiteralNode() {
            /* void */
        }

        void setLiteralData(const NativeType &type) { m_literalData = type; }

    public:
        virtual Node *_generateNode(IrContextTree *context, NodeProgram *program, NodeContainer *container) {
            Node *newNode = IrNode::_generateNode(context, program, container);
            
            if (!newNode->isLiteral()) {
                throw InvalidLiteralType();
            }
            else {
                LiteralNode<NativeType> *literalNode =
                    static_cast<LiteralNode<NativeType> *>(newNode);

                // Inject the literal data
                literalNode->setData(m_literalData);
            }

            return newNode;
        }

    protected:
        NativeType m_literalData;

    protected:
        static void reset(piranha::native_bool &s) { s = false; }
        static void reset(piranha::native_float &s) { s = (piranha::native_float)0.0; }
        static void reset(piranha::native_int &s) { s = (piranha::native_int)0; }
        static void reset(piranha::native_string &s) { s = ""; }
        static void reset(piranha::native_vector &s) { s = { 0.0, 0.0, 0.0, 0.0 }; }
    };

} /* namespace piranha */

#endif /* PIRANHA_IR_LITERAL_NODE_H */
