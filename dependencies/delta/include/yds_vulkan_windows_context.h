#ifndef YDS_VULKAN_WINDOWS_CONTEXT_H
#define YDS_VULKAN_WINDOWS_CONTEXT_H

#include "yds_vulkan_context.h"

class ysVulkanDevice;
class ysVulkanWindowsContext : public ysVulkanContext {
    friend class ysVulkanDevice;

public:
    ysVulkanWindowsContext();
    virtual ~ysVulkanWindowsContext();

    virtual ysError Create(ysVulkanDevice *device, ysWindow *window);
    virtual ysError Destroy();
};

#endif /* YDS_VULKAN_WINDOWS_CONTEXT_H */
