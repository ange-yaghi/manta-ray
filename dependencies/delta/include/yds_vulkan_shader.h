#ifndef YDS_VULKAN_SHADER_H
#define YDS_VULKAN_SHADER_H

#include "yds_shader.h"

class ysVulkanShader : public ysShader {
    friend class ysVulkanDevice;

public:
    ysVulkanShader();
    virtual ~ysVulkanShader();
};

#endif /* YDS_VULKAN_SHADER_H */
