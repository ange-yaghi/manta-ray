#ifndef MANTARAY_CACHEABLE_INPUT_H
#define MANTARAY_CACHEABLE_INPUT_H

#include "node_cache.h"
#include "vector_node_output.h"

#include <piranha.h>

namespace manta {

    template <typename T_Data, typename T_NodeOutput = manta::VectorNodeOutput>
    class CacheableInput {
    public:
        CacheableInput() {
            m_optimizedOut = false;
            m_port = nullptr;
            m_cachedValue = math::constants::Zero;
        }

        ~CacheableInput() {
            /* void */
        }

        const T_Data &sample(const IntersectionPoint *surfaceInteraction) {
            if (m_optimizedOut || m_port == nullptr) return m_cachedValue;
            else {
                
                const T_Data *data = m_cache.cacheGet(surfaceInteraction->m_id, surfaceInteraction->m_threadId);
                if (data == nullptr) {
                    // There was a cache miss
                    T_Data *newData = m_cache.cachePut(surfaceInteraction->m_id, surfaceInteraction->m_threadId);
                    static_cast<T_NodeOutput *>(m_port)->sample(surfaceInteraction, (void *)newData);

                    data = newData;
                }

                return *data;
            }
        }

        void optimize() {
            if (m_port != nullptr) {
                const bool isConstant = m_port->getParentNode()->hasFlag(piranha::Node::META_CONSTANT);

                if (isConstant) {
                    T_Data constantValue;
                    T_NodeOutput *inputNode = static_cast<T_NodeOutput *>(m_port);
                    inputNode->sample(nullptr, (void *)&constantValue);

                    m_optimizedOut = true;
                    m_cachedValue = constantValue;
                }
            }
        }

        void setConstant(const T_Data &value) {
            m_cachedValue = value;
            m_optimizedOut = true;
        }

        const T_Data &getConstant() const {
            return m_cachedValue;
        }

        void setPort(piranha::pNodeInput input) {
            m_port = input;
        }

        piranha::pNodeInput *getPortAddress() {
            return &m_port;
        }

        piranha::pNodeInput getPort() const {
            return m_port;
        }

        void setDefault(const T_Data &data) {
            m_cachedValue = data;
        }

        const T_Data &getDefault() const {
            return m_cachedValue;
        }

    protected:
        NodeCache<T_Data> m_cache;
        piranha::pNodeInput m_port;

        T_Data m_cachedValue;
        bool m_optimizedOut;
    };

} /* namespace manta */

#endif /* MANTARAY_CACHEABLE_INPUT_H */
