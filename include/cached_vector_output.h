#ifndef MANTARAY_CACHED_VECTOR_OUTPUT_H
#define MANTARAY_CACHED_VECTOR_OUTPUT_H

#include "vector_node_output.h"

namespace manta {

    class CachedVectorOutput : public VectorNodeOutput {
    public:
        CachedVectorOutput();
        ~CachedVectorOutput();

        virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
        virtual void discreteSample2d(int x, int y, void *target) const;
        virtual void fullOutput(const void **target) const;

        void setValue(const math::Vector &v) { m_value = v; }
        math::Vector getValue() const { return m_value; }

    protected:
        math::Vector m_value;
    };

} /* namespcae manta */

#endif /* MANTARAY_CACHED_VECTOR_OUTPUT_H */
