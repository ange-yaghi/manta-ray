#ifndef MANTARAY_CACHED_VECTOR_NODE_H
#define MANTARAY_CACHED_VECTOR_NODE_H

#include <piranha.h>

#include "cached_vector_output.h"

namespace manta {

    class CachedVectorNode : public piranha::Node {
    public:
        CachedVectorNode();
        CachedVectorNode(const math::Vector &v);
        virtual ~CachedVectorNode();

        CachedVectorOutput *getMainOutput() { return &m_output; }

        void setValue(const math::Vector &v) { m_output.setValue(v); }
        math::Vector getValue() const { return m_output.getValue(); }

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual piranha::Node *_optimize();

        virtual void registerOutputs();

    protected:
        CachedVectorOutput m_output;
    };

} /* namespace manta */

#endif /* MANTARAY_CACHED_VECTOR_NODE_H */
