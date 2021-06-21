#ifndef YDS_VULKAN_CONTEXT_H
#define YDS_VULKAN_CONTEXT_H

#include "yds_window.h"
#include "yds_rendering_context.h"

#include "yds_vulkan_decl.h"

class ysVulkanDevice;

class ysVulkanContext : public ysRenderingContext {
    friend ysVulkanDevice;

public:
    ysVulkanContext();
    ysVulkanContext(ysWindowSystemObject::Platform platform);
    virtual ~ysVulkanContext();

    virtual ysError Create(ysVulkanDevice *device, ysWindow *window);
    virtual ysError Destroy() = 0;

    VkSurfaceKHR GetSurface() const { return m_surface; }

protected:
    ysVulkanDevice *m_device;

    VkSurfaceKHR m_surface;
};

#endif /* YDS_VULKAN_CONTEXT_H */
