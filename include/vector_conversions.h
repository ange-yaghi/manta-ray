#ifndef MANTARAY_VECTOR_CONVERSIONS_H
#define MANTARAY_VECTOR_CONVERSIONS_H

#include <piranha.h>

#include "vector_node_output.h"
#include "node.h"
#include "cached_vector_node.h"

namespace manta {

    class FloatToVectorConversionOutput : public VectorNodeOutput {
    public:
        FloatToVectorConversionOutput();
        ~FloatToVectorConversionOutput();

        virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
        virtual void discreteSample2d(int x, int y, void *target) const;

        virtual void registerInputs();

        piranha::pNodeInput *getInputConnection() { return &m_input; }

    protected:
        piranha::pNodeInput m_input;
    };

    class IntToVectorConversionOutput : public VectorNodeOutput {
    public:
        IntToVectorConversionOutput();
        ~IntToVectorConversionOutput();

        virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
        virtual void discreteSample2d(int x, int y, void *target) const;

        virtual void registerInputs();

        piranha::pNodeInput *getInputConnection() { return &m_input; }

    protected:
        piranha::pNodeInput m_input;

    protected:
        virtual piranha::Node *_optimize(piranha::NodeAllocator *nodeAllocator);
    };

    template <typename T_VectorConversionOutput>
    class VectorConversion : public Node {
    public:
        VectorConversion() {
            /* void */
        }

        ~VectorConversion() {
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

        virtual void registerOutputs() {
            setPrimaryOutput("__out");
            registerOutput(&m_output, "__out");
        }

        virtual void registerInputs() {
            registerInput(m_output.getInputConnection(), "__in");
        }

    protected:
        virtual piranha::Node *_optimize(piranha::NodeAllocator *nodeAllocator) {
            addFlag(piranha::Node::META_ACTIONLESS);

            VectorNodeOutput *input = static_cast<VectorNodeOutput *>(*m_output.getInputConnection());

            const bool isConstant = input->getParentNode()->hasFlag(piranha::Node::META_CONSTANT);
            if (isConstant) {
                addFlag(piranha::Node::META_CONSTANT);

                const bool result = evaluate();
                if (!result) return nullptr;

                math::Vector constantValue;
                m_output.sample(nullptr, (void *)&constantValue);

                CachedVectorNode *cached = nodeAllocator->allocate<CachedVectorNode>();
                cached->setValue(constantValue);
                mapOptimizedPort(cached, "__out", "__out");

                return cached;
            }

            return this;
        }

        T_VectorConversionOutput m_output;
    };

    typedef VectorConversion<FloatToVectorConversionOutput> FloatToVectorConversionNode;
    typedef VectorConversion<IntToVectorConversionOutput> IntToVectorConversionNode;

} /* namespace manta */

#endif /* MANTARAY_VECTOR_CONVERSIONS_H */
