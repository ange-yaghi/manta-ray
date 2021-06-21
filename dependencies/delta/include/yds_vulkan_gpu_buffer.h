#ifndef YDS_VULKAN_GPU_BUFFER_H
#define YDS_VULKAN_GPU_BUFFER_H

#include "yds_gpu_buffer.h"

class ysVulkanGPUBuffer : public ysGPUBuffer {
    friend class ysVulkanDevice;

public:
    ysVulkanGPUBuffer();
    virtual ~ysVulkanGPUBuffer();
};

#endif /* YDS_VULKAN_GPU_BUFFER_H */
