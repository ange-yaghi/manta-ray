#ifndef YDS_ARRAY_H
#define YDS_ARRAY_H

#include <memory>

#include "yds_error_codes.h"

class ysDynamicArrayElement
{

public:

    ysDynamicArrayElement()
    {

        m_index = -1;
        m_alignment = 0;

    }

    ~ysDynamicArrayElement()
    {
    }

    inline int GetIndex() const
    {

        return m_index;

    }

    inline void SetIndex(int index)
    {

        m_index = index;

    }

    inline int GetAlignment() const
    {

        return m_alignment;

    }

    inline void SetAlignment(int alignment)
    {

        m_alignment = alignment;

    }

protected:

    int m_index;
    int m_alignment;

};

template<typename TYPE, int START_SIZE=0>
class ysDynamicArray
{

public:

    ysDynamicArray()
    {

        m_maxSize = START_SIZE;
        m_nObjects = 0;
        m_array = NULL;

        Preallocate(m_maxSize);

    }

    ~ysDynamicArray()
    {

        Clear();
        delete [] m_array;

    }

    void Preallocate(int nObjects)
    {

        if (!nObjects) return;
        if (m_array) return;

        m_array = new TYPE *[nObjects];

    }
    
    TYPE *New()
    {

        if (m_nObjects >= m_maxSize) Extend();

        m_array[m_nObjects] = new TYPE;

        // Cast to a standard array element
        ysDynamicArrayElement *sElement = static_cast<ysDynamicArrayElement *>(m_array[m_nObjects]);
        sElement->SetAlignment(0);
        sElement->SetIndex(m_nObjects);

        return m_array[m_nObjects++];

    }

    void Add(TYPE *type)
    {

        if (m_nObjects >= m_maxSize) Extend();

        m_array[m_nObjects] = type;

        // Cast to a standard array element
        ysDynamicArrayElement *sElement = static_cast<ysDynamicArrayElement *>(m_array[m_nObjects]);
        sElement->SetIndex(m_nObjects);

        m_nObjects++;

    }

    void Add(TYPE *type, int offset)
    {

        if (m_nObjects >= m_maxSize) Extend();

        // Make room
        for (int i=m_nObjects; i > offset; i--)
        {

            m_array[i] = m_array[i-1];

        }

        m_array[offset] = type;

        // Cast to a standard array element
        ysDynamicArrayElement *sElement = static_cast<ysDynamicArrayElement *>(m_array[offset]);
        sElement->SetIndex(offset);

        m_nObjects++;

    }

    template<typename DYN_TYPE>
    DYN_TYPE *NewGeneric(int alignment=0)
    {

        if (m_nObjects >= m_maxSize) Extend();

        if (alignment != 0)
        {

            void *memory = _aligned_malloc(sizeof(DYN_TYPE), alignment);
            m_array[m_nObjects] = static_cast<TYPE *>(new (memory) DYN_TYPE);

        }

        else m_array[m_nObjects] = static_cast<TYPE *>(new DYN_TYPE);

        // Cast to a standard array element
        ysDynamicArrayElement *sElement = static_cast<ysDynamicArrayElement *>(m_array[m_nObjects]);
        sElement->SetAlignment(alignment);
        sElement->SetIndex(m_nObjects);

        return static_cast<DYN_TYPE *>(m_array[m_nObjects++]);

    }

    ysError Delete(int index, bool destroy=true, TYPE *replacement=NULL, bool preserveOrder=false)
    {

        if (index >= m_nObjects || index < 0) return ysError::OutOfBounds;

        if (m_nObjects <= m_maxSize / 2) Condense();

        ysDynamicArrayElement *target = static_cast<ysDynamicArrayElement *>(m_array[index]);

        if (destroy)
        {

            if (target->GetAlignment() == 0)
            {

                delete m_array[index];

            }

            else
            {

                m_array[index]->~TYPE();
                _aligned_free(m_array[index]);

            }

        }

        if (replacement == NULL)
        {

            if (!preserveOrder)
            {

                m_array[index] = m_array[m_nObjects-1];
                m_array[m_nObjects - 1] = NULL;

            }

            else
            {

                for (int i=index; i < m_nObjects - 1; i++)
                {

                    m_array[i] = m_array[i + 1];

                    ysDynamicArrayElement *sElement = static_cast<ysDynamicArrayElement *>(m_array[i]);
                    sElement->SetIndex(i);

                }

                // Just as a precaution
                m_array[m_nObjects - 1] = NULL;

            }

        }

        else
        {

            m_array[index] = replacement;

        }

        // Cast to a standard array element
        if (m_array[index])
        {

            ysDynamicArrayElement *sElement = static_cast<ysDynamicArrayElement *>(m_array[index]);
            sElement->SetIndex(index);

        }

        if (replacement == NULL)
        {

            m_nObjects--;

        }

        return ysError::None;

    }

    inline TYPE * Get(int index) const
    {

        return m_array[index];

    }

    inline TYPE **GetBuffer()
    {

        return m_array;

    }

    int GetNumObjects() const
    {

        return m_nObjects;

    }

    void Clear(bool destroy=true)
    {

        if (destroy)
        {

            int nObjects = m_nObjects;
            for(int i = nObjects-1; i >= 0; i--)
            {

                Delete(i);

            }

        }

        m_nObjects = 0;

    }

protected:

    void Extend()
    {

        TYPE **newArray = new TYPE *[m_maxSize * 2 + 1];
        memcpy(newArray, m_array, sizeof(TYPE *) * m_nObjects);

        delete [] m_array;

        m_array = newArray;
        m_maxSize *= 2;
        m_maxSize++;

    }

    void Condense()
    {

        TYPE **newArray = new TYPE *[m_maxSize / 2 + 1];
        memcpy(newArray, m_array, sizeof(TYPE *) * m_nObjects);

        delete [] m_array;

        m_array = newArray;
        m_maxSize /= 2;
        m_maxSize++;

    }

protected:

    TYPE **m_array;
    int m_maxSize;
    int m_nObjects;

};

#endif