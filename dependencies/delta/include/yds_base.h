#ifndef YDS_BASE_H
#define YDS_BASE_H

#include "yds_dynamic_array.h"

class ysObject : public ysDynamicArrayElement {
public:
    ysObject();
    ysObject(const char *typeID);
    ~ysObject();

//#ifdef _DEBUG
//    void RaiseError(bool condition, const char *format, ...);
//#else
//#define RaiseError(condition, format, ...) ((void)0)
//#endif

    const char *GetTypeID() const { return m_typeID; }

protected:
    const char *m_typeID;
};

// TEMP
#include "yds_error_system.h"

#endif /* YDS_BASE_H */
