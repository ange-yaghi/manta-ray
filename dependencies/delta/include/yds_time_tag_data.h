#ifndef YDS_TIME_TAG_DATA_H
#define YDS_TIME_TAG_DATA_H

#include "yds_expanding_array.h"

class ysTimeTagData {
public:
    struct TimeTag {
        char Name[64];
        int Frame;
    };

public:
    ysTimeTagData();
    ~ysTimeTagData();

    void Clear();

    int m_timeTagCount;
    ysExpandingArray<TimeTag, 1> m_timeTags;
};

#endif /* YDS_TIME_TAG_DATA_H */
