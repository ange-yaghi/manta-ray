#ifndef YDS_VULKAN_INPUT_LAYOUT_H
#define YDS_VULKAN_INPUT_LAYOUT_H

#include "yds_input_layout.h"

class ysVulkanInputLayout : public ysInputLayout {
    friend class ysVulkanDevice;

public:
    ysVulkanInputLayout();
    virtual ~ysVulkanInputLayout();
};

#endif /* YDS_VULKAN_INPUT_LAYOUT_H */
