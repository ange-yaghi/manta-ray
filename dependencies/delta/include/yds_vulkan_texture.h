#ifndef YDS_VULKAN_TEXTURE_H
#define YDS_VULKAN_TEXTURE_H

#include "yds_texture.h"

class ysVulkanTexture : public ysTexture {
    friend class ysVulkanDevice;

public:
    ysVulkanTexture();
    virtual ~ysVulkanTexture();
};

#endif /* YDS_VULKAN_TEXTURE_H */
