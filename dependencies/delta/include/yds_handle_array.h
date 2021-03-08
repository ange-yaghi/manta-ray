#ifndef YDS_HANDLE_ARRAY_H
#define YDS_HANDLE_ARRAY_H

#include "yds_dynamic_array.h"

#include "yds_linked_list.h"

template<typename TYPE, int START_SIZE=0>
class ysHandleArray : public ysDynamicArray<TYPE, START_SIZE> {
public:
    ysHandleArray() { /* void */ }
    ~ysHandleArray() { /* void */  }

    void Preallocate(int nObjects) {
        if (nObjects == 0) return;
        if (m_array != nullptr) return;

        m_array = new TYPE[nObjects];
    }
    
    int New() {
        if (m_nObjects >= m_maxSize) Extend();

        m_array[m_nObjects] = new TYPE;
        return m_nObjects++;
    }

    void Delete(int index) {
        if (m_nObjects <= m_maxSize / 2) Condense();

        delete m_array[index];
        m_array[index] = NULL;
        m_nObjects--;
    }

    TYPE *Get(int index) {
        return m_array[index];
    }

protected:
    void Extend() {
        TYPE **newArray = new TYPE *[m_maxSize * 2];
        memcpy(newArray, m_array, sizeof(TYPE *) * m_nObjects);

        delete [] m_array;

        m_array = newArray;
    }

    void Condense() {
        TYPE **newArray = new TYPE *[m_maxSize / 2];
        memcpy(newArray, m_array, sizeof(TYPE *) * m_nObjects);

        delete [] m_array;

        m_array = newArray;
    }

protected:
    ysLinkedList m_slots;
};

#endif /* YDS_HANDLE_ARRAY_H */
