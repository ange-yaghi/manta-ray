#ifndef PIRANHA_FLOAT_CONVERSIONS_H
#define PIRANHA_FLOAT_CONVERSIONS_H

#include "fundamental_output.h"

#include "pipe_node.h"
#include "default_literal_node.h"

namespace piranha {

    class StringToFloatConversionOutput : public FloatValueOutput {
    public:
        StringToFloatConversionOutput();
        ~StringToFloatConversionOutput();

        virtual void fullCompute(void *target) const;
        virtual void registerInputs();

        pNodeInput *getInputConnection() { return &m_input; }

    protected:
        pNodeInput m_input;
    };
    

    class IntToFloatConversionOutput : public FloatValueOutput {
    public:
        IntToFloatConversionOutput();
        ~IntToFloatConversionOutput();

        virtual void fullCompute(void *target) const;
        virtual void registerInputs();

        pNodeInput *getInputConnection() { return &m_input; }

    protected:
        pNodeInput m_input;
    };

    template <typename T_ConversionOutput>
    class FloatConversion : public Node {
    public:
        FloatConversion() {
            /* void */
        }

        ~FloatConversion() {
            /* void */
        }

        virtual void _initialize() {
            m_output.initialize();
        }

        virtual void _evaluate() {
            /* void */
        }

        virtual void _destroy() {
            /* void */
        }

        virtual void registerInputs() {
            registerInput(m_output.getInputConnection(), "__in");
        }

        virtual void registerOutputs() {
            setPrimaryOutput("__out");
            registerOutput(&m_output, "__out");
        }

    protected:
        virtual Node *_optimize(NodeAllocator *nodeAllocator) {
            addFlag(Node::META_ACTIONLESS);

            bool isConstant = true;
            pNodeInput input = *m_output.getInputConnection();
            if (!input->getParentNode()->hasFlag(META_CONSTANT)) {
                isConstant = false;
            }

            if (isConstant) {
                addFlag(Node::META_CONSTANT);

                const bool result = evaluate();
                if (!result) return nullptr;

                DefaultLiteralNode<piranha::native_float> *newLiteral =
                    nodeAllocator->allocate<DefaultLiteralNode<piranha::native_float>>();

                piranha::native_float computedValue;
                m_output.fullCompute((void *)&computedValue);

                mapOptimizedPort(newLiteral, "__out", "__out");
                newLiteral->setData(computedValue);
                return newLiteral;
            }

            return this;
        }

        T_ConversionOutput m_output;
    };

    typedef FloatConversion<IntToFloatConversionOutput> IntToFloatConversionNode;
    typedef FloatConversion<StringToFloatConversionOutput> StringToFloatConversionNode;

} /* namespace piranha */

#endif /* PIRANHA_FLOAT_CONVERSIONS_H */
