//
// Created by jonah on 8/5/2023.
//

#ifndef VULKAN_MODEL_H
#define VULKAN_MODEL_H


#include <vulkan/vulkan.h>
#include <vector>
#include "vk/pipeline/gra_pipeline.h"

// super inefficient first model version
class Model {

private:
    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkDescriptorSet> descriptorSets{};

    Raster::Pipeline pipeline{};

public:
    Model();
    void destroy();

};


#endif //VULKAN_MODEL_H
