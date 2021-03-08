#ifndef YDS_DYNAMIC_ARRAY_H
#define YDS_DYNAMIC_ARRAY_H

#include "yds_error_codes.h"
#include "yds_allocator.h"

#include <memory>

class ysDynamicArrayElement {
public:
    ysDynamicArrayElement() {
        m_index = -1;
        m_alignment = 0;
    }

    ~ysDynamicArrayElement() {
        /* void */
    }

    inline int GetIndex() const {
        return m_index;
    }

    inline void SetIndex(int index) {
        m_index = index;
    }

    inline int GetAlignment() const {
        return m_alignment;
    }

    inline void SetAlignment(int alignment) {
        m_alignment = alignment;
    }

protected:
    int m_index;
    int m_alignment;
};

template<typename TYPE, int START_SIZE = 0>
class ysDynamicArray {
public:
    ysDynamicArray() {
        m_maxSize = START_SIZE;
        m_nObjects = 0;
        m_array = nullptr;

        Preallocate(m_maxSize);
    }

    ~ysDynamicArray() {
        Clear();
        delete[] m_array;
    }

    void Preallocate(int nObjects) {
        if (!nObjects) return;
        if (m_array) return;

        m_array = new TYPE * [nObjects];
    }

    TYPE *New() {
        if (m_nObjects >= m_maxSize) Extend();

        m_array[m_nObjects] = ysAllocator::TypeAllocate<TYPE, 1>();

        // Cast to a standard array element
        ysDynamicArrayElement *sElement = static_cast<ysDynamicArrayElement *>(m_array[m_nObjects]);
        sElement->SetAlignment(1);
        sElement->SetIndex(m_nObjects);

        return m_array[m_nObjects++];
    }

    void Add(TYPE *type) {
        if (m_nObjects >= m_maxSize) Extend();

        m_array[m_nObjects] = type;

        // Cast to a standard array element
        ysDynamicArrayElement *sElement = static_cast<ysDynamicArrayElement *>(m_array[m_nObjects]);
        sElement->SetIndex(m_nObjects);

        m_nObjects++;
    }

    void Add(TYPE *type, int offset) {
        if (m_nObjects >= m_maxSize) Extend();

        // Make room
        for (int i = m_nObjects; i > offset; i--) {
            m_array[i] = m_array[i - 1];
        }

        m_array[offset] = type;

        // Cast to a standard array element
        ysDynamicArrayElement *sElement = static_cast<ysDynamicArrayElement *>(m_array[offset]);
        sElement->SetIndex(offset);

        m_nObjects++;
    }

    void Move(TYPE *type, int offset) {
        // Make room
        for (int i = m_nObjects - 1; i > offset; i--) {
            m_array[i] = m_array[i - 1];
        }

        m_array[offset] = type;

        // Cast to a standard array element
        ysDynamicArrayElement *sElement = static_cast<ysDynamicArrayElement *>(m_array[offset]);
        sElement->SetIndex(offset);
    }

    template<typename T_Create, int Alignment = 1>
    T_Create *NewGeneric() {
        if (m_nObjects >= m_maxSize) Extend();

        T_Create *newObject = ysAllocator::TypeAllocate<T_Create, Alignment>();

        // Cast to a standard array element
        ysDynamicArrayElement *sElement = static_cast<ysDynamicArrayElement *>(newObject);
        sElement->SetAlignment(Alignment);
        sElement->SetIndex(m_nObjects);

        m_array[m_nObjects++] = newObject;

        return newObject;
    }

    ysError Delete(int index, bool destroy = true, TYPE *replacement = nullptr, bool preserveOrder = false) {
        if (index >= m_nObjects || index < 0) return ysError::OutOfBounds;

        if (m_nObjects <= m_maxSize / 2) Condense();

        ysDynamicArrayElement *target = static_cast<ysDynamicArrayElement *>(m_array[index]);

        if (destroy) {
            ysAllocator::TypeFree<TYPE>(m_array[index], 1, true, target->GetAlignment());
        }

        if (replacement == nullptr) {
            if (!preserveOrder) {
                m_array[index] = m_array[m_nObjects - 1];
                m_array[m_nObjects - 1] = nullptr;
            }
            else {
                for (int i = index; i < m_nObjects - 1; i++) {
                    m_array[i] = m_array[i + 1];

                    ysDynamicArrayElement *sElement = static_cast<ysDynamicArrayElement *>(m_array[i]);
                    sElement->SetIndex(i);
                }

                // Just as a precaution
                m_array[m_nObjects - 1] = nullptr;
            }
        }
        else {
            m_array[index] = replacement;
        }

        // Cast to a standard array element
        if (m_array[index]) {
            ysDynamicArrayElement *sElement = static_cast<ysDynamicArrayElement *>(m_array[index]);
            sElement->SetIndex(index);
        }

        if (replacement == nullptr) {
            m_nObjects--;
        }

        return ysError::None;
    }

    inline TYPE *Get(int index) const {
        return m_array[index];
    }

    inline TYPE **GetBuffer() {
        return m_array;
    }

    int GetNumObjects() const {
        return m_nObjects;
    }

    void Clear(bool destroy = true) {
        if (destroy) {
            int nObjects = m_nObjects;
            for (int i = nObjects - 1; i >= 0; --i) {
                Delete(i);
            }
        }

        m_nObjects = 0;
    }

protected:
    void Extend() {
        TYPE **newArray = new TYPE * [m_maxSize * 2 + 1];
        memcpy(newArray, m_array, sizeof(TYPE *) * m_nObjects);

        delete[] m_array;

        m_array = newArray;
        m_maxSize *= 2;
        m_maxSize++;
    }

    void Condense() {
        TYPE **newArray = new TYPE * [m_maxSize / 2 + 1];
        memcpy(newArray, m_array, sizeof(TYPE *) * m_nObjects);

        delete[] m_array;

        m_array = newArray;
        m_maxSize /= 2;
        m_maxSize++;
    }

protected:
    TYPE **m_array;
    int m_maxSize;
    int m_nObjects;
};

#endif /* YDS_DYNAMIC_ARRAY_H */
