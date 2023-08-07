//
// Created by jonah on 8/5/2023.
//

#ifndef VULKAN_MODEL_H
#define VULKAN_MODEL_H


#include <vulkan/vulkan.h>

// super inefficient first model version
class Model {

private:
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipeline graphicsPipeline;

public:
    Model();
    void destroy();
};


#endif //VULKAN_MODEL_H
