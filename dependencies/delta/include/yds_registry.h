#ifndef YDS_REGISTRY_H
#define YDS_REGISTRY_H

#include "yds_base.h"
#include "yds_dynamic_array.h"

template<typename TYPE, int START_SIZE = 0>
class ysRegistry : public ysObject {
public:
    ysRegistry() : ysObject("Registry") {
        m_maxSize = START_SIZE;
        m_nObjects = 0;
        m_array = NULL;

        Preallocate(m_maxSize);
    }

    ~ysRegistry() {
        delete[] m_array;
    }

    void Preallocate(int nObjects) {
        if (!nObjects) return;
        if (m_array) return;

        m_array = new TYPE * [nObjects];
    }

    void Register(TYPE *obj) {
        if (m_nObjects >= m_maxSize) Extend();

        m_array[m_nObjects] = obj;

        // Cast to a standard array element
        ysDynamicArrayElement *sElement = static_cast<ysDynamicArrayElement *>(m_array[m_nObjects]);
        sElement->SetAlignment(0);
        sElement->SetIndex(m_nObjects);

        m_nObjects++;
    }

    template<typename DYN_TYPE>
    void NewGeneric(DYN_TYPE *obj, int alignment = 0) {
        if (m_nObjects >= m_maxSize) Extend();

        m_array[m_nObjects] = static_cast<TYPE *>(obj);

        // Cast to a standard array element
        ysDynamicArrayElement *sElement = static_cast<ysDynamicArrayElement *>(m_array[m_nObjects]);
        sElement->SetAlignment(alignment);
        sElement->SetIndex(m_nObjects);

        return static_cast<DYN_TYPE *>(m_array[m_nObjects++]);
    }

    ysError Remove(int index) {
        YDS_ERROR_DECLARE("Delete");

        if (index >= m_nObjects || index < 0) return YDS_ERROR_RETURN(ysError::OutOfBounds);

        if (m_nObjects <= m_maxSize / 2) Condense();

        ysDynamicArrayElement *target = static_cast<ysDynamicArrayElement *>(m_array[index]);

        m_array[index] = m_array[m_nObjects - 1];
        m_array[m_nObjects - 1] = NULL;

        // Cast to a standard array element
        if (m_array[index]) {
            ysDynamicArrayElement *sElement = static_cast<ysDynamicArrayElement *>(m_array[index]);
            sElement->SetIndex(index);
        }

        m_nObjects--;

        return YDS_ERROR_RETURN(ysError::None);
    }

    inline TYPE *Get(int index) {
        return m_array[index];
    }

    int GetNumObjects() const {
        return m_nObjects;
    }

    void Clear() {
        for (int i = 0; i < m_nObjects; i++) {
            m_array[i] = NULL;
        }

        m_nObjects = 0;
    }

protected:
    void Extend() {
        TYPE **newArray = new TYPE * [m_maxSize * 2];
        memcpy(newArray, m_array, sizeof(TYPE *) * m_nObjects);

        delete[] m_array;

        m_array = newArray;
        m_maxSize *= 2;
    }

    void Condense() {
        TYPE **newArray = new TYPE * [m_maxSize / 2];
        memcpy(newArray, m_array, sizeof(TYPE *) * m_nObjects);

        delete[] m_array;

        m_array = newArray;
        m_maxSize /= 2;
    }

protected:
    TYPE **m_array;
    int m_maxSize;
    int m_nObjects;
};

#endif /* YDS_REGISTRY_H */
