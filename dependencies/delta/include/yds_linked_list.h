#ifndef YDS_LINKED_LIST_H
#define YDS_LINKED_LIST_H

#include "yds_memory_base.h"

class ysLink {
friend class ysLinkedList;
public:
    ysLink();
    ~ysLink();

    union {
        void *Data;
        int *IntData;
    };

    ysLink *GetNext() { return Next; }
    ysLink *GetPrevious() { return Previous; }

protected:
    ysLink *Next;
    ysLink *Previous;
};

class ysLinkedListIterator {
public:
    ysLinkedListIterator(ysLink *start);
    ysLinkedListIterator();
    ~ysLinkedListIterator();

    void SetStart(ysLink *start) { 
        m_start = start; 
    }

    void Reset(ysLink *start) { SetStart(start); Reset(); }
    void Reset() { m_location = m_start; m_reachedStart = false; }

    void Increment() {
        m_location = m_location->GetNext();
        if (m_location == m_start) m_reachedStart = true;
        else m_reachedStart = false;
    }

    void Decrement() {
        m_location = m_location->GetNext();
        if (m_location == m_start) m_reachedStart = true;
        else m_reachedStart = false;
    }

    bool AtEnd() {
        return m_reachedStart;
    }

    ysLink *GetLink() { return m_location; }

protected:
    ysLink *m_start;
    ysLink *m_location;

    bool m_reachedStart;
};

class ysLinkedList {
public:
    ysLinkedList();
    ~ysLinkedList();

    void AppendLink(ysLink *link);
    void PrependLink(ysLink *link);
    void DeleteLink(ysLink *link);
    
    ysLink *GetHead();
    ysLink *GetTail();

protected:
    ysLink *m_head;
};

#endif /* YDS_LINKED_LIST_H */
