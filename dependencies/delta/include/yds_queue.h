#ifndef YDS_QUEUE_H
#define YDS_QUEUE_H

#include <stdlib.h>

template<typename Type, int InitialSize = 0, int Alignment = 1>
class ysQueue {
public:
    ysQueue() {
        m_maxSize = InitialSize;
        m_objectCount = 0;
        m_start = 0;
        m_array = nullptr;
    }

    ~ysQueue() { /* void */ }

    inline Type &Append() {
        if (m_objectCount >= m_maxSize) Extend();

        Type *newObject = (new ((void *)&m_array[(m_start + m_objectCount) % m_objectCount]) Type);

        m_objectCount++;

        return newObject;
    }

    inline Type &Prepend() {
        if (m_objectCount >= m_maxSize) Extend();

        Type *newObject = (new ((void *)&m_array[(m_start + m_objectCount) % m_objectCount]) Type);

        m_objectCount++;

        m_start += m_maxSize;
        m_start--;
        m_start %= m_maxSize;

        return newObject;
    }

    Type *GetBuffer() { return m_array; }

    __forceinline Type &operator[](int index) {
        return m_array[(index + m_start) % m_maxSize];
    }

    void DeleteFront() {
        m_start++;
        m_start %= m_maxSize;
        m_objectCount--;
    }

    void DeleteBack() {
        m_objectCount--;
    }

    void Clear() {
        m_objectCount = 0;
        m_start = 0;
    }

    void Destroy() {
        DestroyArray(m_array);

        m_maxSize = 0;
        m_objectCount = 0;
        m_array = NULL;
        m_start = 0;
    }

protected:
    Type *CreateArray(int size) {
        Type *ret = NULL;

        if (Alignment != 1) {
            void *memory = _aligned_malloc(sizeof(Type) * size, Alignment);
            ret = (Type *)memory;
        }
        else ret = new Type[size];

        return ret;
    }

    void DestroyArray(Type *arr) {
        if (Alignment == 1) {
            delete[] arr;
        }
        else {
            for (int i = 0; i < m_maxSize; i++) {
                arr[i].~Type();
            }

            _aligned_free(arr);
        }
    }

    void Allocate(int size) {
        if (size == 0) return;
        if (m_array != NULL) {
            if (m_maxSize < size) {
                Destroy();
            }
            else {
                m_objectCount = size;
                return;
            }
        }

        m_array = CreateArray(size);
        m_maxSize = size;
        m_objectCount = size;
        m_start = 0;
    }

    void Preallocate(int size) {
        if (size == 0) return;
        if (m_array != NULL) {
            if (m_maxSize < size) {
                Destroy();
            }
            else {
                // Already allcoated
                m_objectCount = 0;
                return;
            }
        }

        m_array = CreateArray(size);
        m_maxSize = size;
        m_objectCount = 0;
        m_start = 0;
    }

    void Extend() {
        Type *newArray = CreateArray(m_maxSize * 2 + 1);

        int modIndex = m_start;
        for (int i = 0; i < m_nObjects; i++) {
            newArray[i] = m_array[i];

            modIndex++;
            modIndex %= m_maxSize;
        }

        m_start = 0;

        DestroyArray(m_array);

        m_array = newAray;
        m_maxSize *= 2;
        m_maxSize += 1;
    }

protected:
    int m_maxSize;
    int m_start;
    int m_objectCount;

    Type *m_array;
};

#endif /* YDS_QUEUE_H */
