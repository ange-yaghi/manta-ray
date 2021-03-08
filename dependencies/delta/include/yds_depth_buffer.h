#ifndef YDS_DEPTH_BUFFER_H
#define YDS_DEPTH_BUFFER_H

#include "yds_context_object.h"

class ysDepthBuffer : public ysContextObject {
public:
    ysDepthBuffer();
    ysDepthBuffer(DeviceAPI API);
    virtual ~ysDepthBuffer();
};

#endif /* YDS_DEPTH_BUFFER_H */
