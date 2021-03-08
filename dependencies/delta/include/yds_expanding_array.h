#ifndef YDS_EXPANDING_ARRAY_H
#define YDS_EXPANDING_ARRAY_H

#include "yds_allocator.h"

#include <stdlib.h>
#include <new>

template<typename TYPE, int START_SIZE = 0, int ALIGNMENT = 1>
class ysExpandingArray {
public:
    ysExpandingArray() {
        m_maxSize = START_SIZE;
        m_nObjects = 0;
        m_array = nullptr;

        Preallocate(m_maxSize);
    }

    ysExpandingArray(const ysExpandingArray &ref) {
        /* void */
    }

    ysExpandingArray &operator=(const ysExpandingArray &ref) {
        Preallocate(ref.m_maxSize);

        for (int i = 0; i < ref.m_nObjects; i++) {
            m_array[i] = ref.m_array[i];
        }

        m_nObjects = ref.m_nObjects;
        m_maxSize = ref.m_maxSize;

        return *this;
    }

    ~ysExpandingArray() {
        Destroy();
    }

    void Clear() {
        m_nObjects = 0;
    }

    void Destroy() {
        DestroyArray(m_array);

        m_maxSize = 0;
        m_nObjects = 0;
        m_array = nullptr;
    }

    TYPE *CreateArray(int nObjects, bool construct = true) {
        return ysAllocator::TypeAllocate<TYPE, ALIGNMENT>(nObjects, construct);
    }

    void DestroyArray(TYPE *arr) {
        /*if (ALIGNMENT == 1) {
            delete[] arr;
        }
        else {
            for (int i = 0; i < m_maxSize; i++) {
                arr[i].~TYPE();
            }

            _aligned_free(arr);
        }*/
        ysAllocator::TypeFree(arr, m_nObjects, true, ALIGNMENT);
    }

    void Allocate(int nObjects) {
        if (nObjects == 0) return;
        if (m_array != nullptr) {
            if (m_maxSize < nObjects) {
                Destroy();
            }
            else {
                // Already allocated, no sense in wasting time
                m_nObjects = nObjects;
                return;
            }
        }

        m_array = CreateArray(nObjects, true);
        m_maxSize = nObjects;
        m_nObjects = nObjects;
    }

    void Preallocate(int nObjects) {
        if (nObjects == 0) return;
        if (m_array != nullptr) {
            if (m_maxSize < nObjects) {
                Destroy();
            }
            else {
                // Already allocated, no sense in wasting time
                m_nObjects = 0;
                return;
            }
        }

        m_array = CreateArray(nObjects, false);
        m_maxSize = nObjects;
        m_nObjects = 0;
    }

    inline TYPE &New() {
        if (m_nObjects >= m_maxSize) Extend();

        return *(new ((void *)&m_array[m_nObjects++]) TYPE);
    }

    TYPE &Insert(int index) {
        New();

        for (int i = (m_nObjects - 1); i > index; i--) {
            m_array[i] = m_array[i - 1];
        }

        return m_array[index];
    }

    TYPE *GetBuffer() { return m_array; }

    __forceinline TYPE &operator[](int index) {
        return m_array[index];
    }

    TYPE operator[](int index) const {
        return m_array[index];
    }

    void Copy(int index) {
        TYPE &cpy = New();
        cpy = m_array[index];
    }

    int GetNumObjects() const {
        return m_nObjects;
    }

    bool IsActive() const {
        return (m_array != nullptr);
    }

    void Delete(int index, bool maintainOrder = false) {
        if (maintainOrder) {
            for (int i = index; i < m_nObjects; i++) {
                m_array[i] = m_array[i + 1];
            }

            m_array[m_nObjects - 1] = nullptr;
        }
        else {
            m_array[index] = m_array[m_nObjects - 1];
        }

        m_nObjects--;
    }

    int Find(TYPE &ref) const {
        for (int i = 0; i < m_nObjects; i++) {
            if (m_array[i] == ref) return i;
        }

        return -1;
    }

private:
    void Extend() {
        TYPE *newArray = CreateArray(m_maxSize * 2 + 1);
        for (int i = 0; i < m_nObjects; i++) {
            newArray[i] = m_array[i];
        }

        DestroyArray(m_array);

        m_array = newArray;
        m_maxSize *= 2;
        m_maxSize += 1;
    }

    int m_maxSize;
    int m_nObjects;
    TYPE *m_array;
};

#endif /* YDS_EXPANDING_ARRAY_H */
