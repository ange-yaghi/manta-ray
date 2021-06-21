#ifndef YDS_VULKAN_SHADER_PROGRAM_H
#define YDS_VULKAN_SHADER_PROGRAM_H

#include "yds_shader_program.h"

class ysVulkanShader;
class ysVulkanShaderProgram : public ysShaderProgram {
    friend class ysVulkanDevice;

public:
    ysVulkanShaderProgram();
    ~ysVulkanShaderProgram();
};

#endif /* YDS_VULKAN_SHADER_PROGRAM_H */
