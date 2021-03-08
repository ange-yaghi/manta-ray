#ifndef YDS_STAT_H
#define YDS_STAT_H

#include "yds_math.h"

namespace ysStat {

    float RandomNumber();
    bool Decide(float probability);
    bool Decide(float frequency, float timePassed);

    float NormalRandomNumber(float variance);
    float NormalRandomNumber(float mean, float variance);

} /* namespace ysStat */

#endif /* YDS_STAT_H */
