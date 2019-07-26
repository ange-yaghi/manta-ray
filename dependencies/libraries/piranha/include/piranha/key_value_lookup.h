#ifndef PIRANHA_KEY_VALUE_LOOKUP_H
#define PIRANHA_KEY_VALUE_LOOKUP_H

#include <vector>

namespace piranha {

    template <typename KeyType, typename ValueType>
    class KeyValueLookup {
    private:
        struct KeyValuePair {
            KeyType key;
            ValueType *value;
        };

    public:
        KeyValueLookup() {
            /* void */
        }

        ~KeyValueLookup() {
            /* void */
        }

        int getEntryCount() const {
            return (int)m_lookupTable.size();
        }

        ValueType *lookup(const KeyType &key) const {
            int entryCount = getEntryCount();
            for (int i = 0; i < entryCount; i++) {
                if (m_lookupTable[i].key == key) {
                    return m_lookupTable[i].value;
                }
            }

            return nullptr;
        }

        const KeyType &getKey(int index) const {
            return m_lookupTable[index].key;
        }

        ValueType *get(int index) const {
            return m_lookupTable[index].value;
        }

        template <typename T>
        ValueType *newValue(const KeyType &key) {
            KeyValuePair kvp;
            kvp.key = key;
            kvp.value = new T();

            m_lookupTable.push_back(kvp);

            return kvp.value;
        }

        ValueType *newValue(const KeyType &key) {
            KeyValuePair kvp;
            kvp.key = key;
            kvp.value = new ValueType();

            m_lookupTable.push_back(kvp);

            return kvp.value;
        }

    protected:
        std::vector<KeyValuePair> m_lookupTable;
    };

 } /* namespace piranha */

#endif /* PIRANHA_KEY_VALUE_LOOKUP_H */
