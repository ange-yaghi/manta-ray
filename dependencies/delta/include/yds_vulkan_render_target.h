#ifndef YDS_VULKAN_RENDER_TARGET_H
#define YDS_VULKAN_RENDER_TARGET_H

#include "yds_render_target.h"

class ysVulkanRenderTarget : public ysRenderTarget {
    friend class ysVulkanDevice;

public:
    ysVulkanRenderTarget();
    virtual ~ysVulkanRenderTarget();
};

#endif /* YDS_VULKAN_RENDER_TARGET_H */
