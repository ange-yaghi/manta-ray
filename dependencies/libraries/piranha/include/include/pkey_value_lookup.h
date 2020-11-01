#ifndef PIRANHA_PKEY_VALUE_LOOKUP_H
#define PIRANHA_PKEY_VALUE_LOOKUP_H

#include "memory_tracker.h"

#include <vector>

namespace piranha {

    template <typename KeyType, typename ValueType>
    class PKeyValueLookup {
    private:
        struct KeyValuePair {
            const KeyType *key;
            ValueType *value;
        };

    public:
        PKeyValueLookup() {
            /* void */
        }

        ~PKeyValueLookup() {
            /* void */
        }

        int getEntryCount() const {
            return (int)m_lookupTable.size();
        }

        ValueType *lookup(const KeyType *key) const {
            int entryCount = getEntryCount();
            for (int i = 0; i < entryCount; i++) {
                if (key == nullptr || m_lookupTable[i].key == nullptr) {
                    if (key == m_lookupTable[i].key) {
                        return m_lookupTable[i].value;
                    }
                }
                else if (*m_lookupTable[i].key == *key) {
                    return m_lookupTable[i].value;
                }
            }

            return nullptr;
        }

        ValueType *get(int index) const {
            return m_lookupTable[index].value;
        }

        template <typename T>
        ValueType *newValue(const KeyType *key) {
            ValueType *current = lookup(key);
            if (current != nullptr) return current;

            KeyValuePair kvp;
            kvp.key = key;
            kvp.value = TRACK(new T());

            m_lookupTable.push_back(kvp);

            return kvp.value;
        }

        ValueType *newValue(const KeyType *key) {
            return newValue<ValueType>(key);
        }

        void destroy() {
            for (KeyValuePair &kvp : m_lookupTable) {
                delete FTRACK(kvp.value);
            }
        }

    protected:
        std::vector<KeyValuePair> m_lookupTable;
    };

} /* namespace piranha */

#endif /* PIRANHA_PKEY_VALUE_LOOKUP_H */
