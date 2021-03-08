#ifndef YDS_ANIMATION_INTERCHANGE_FILE_READER_H
#define YDS_ANIMATION_INTERCHANGE_FILE_READER_H

#include "yds_base.h"

#include "yds_animation_curve.h"
#include "yds_math.h"

#include <fstream>

class ysAnimationAction;

class ysAnimationInterchangeFileReader : public ysObject {
public:
    ysAnimationInterchangeFileReader();
    ~ysAnimationInterchangeFileReader();

    virtual unsigned int GetActionCount() const = 0;

    virtual ysError ReadHeader(std::fstream &f) = 0;
    virtual ysError ReadAction(std::fstream &f, ysAnimationAction *action) = 0;
};

#endif /* YDS_ANIMATION_INTERCHANGE_FILE_READER_H */
