#pragma once
#include "vk/drawing/gra_drawing.h"
#include "vk/shading/gra_uniform.h"
#include <vector>
#include <vulkan/vulkan_core.h>

/*
 * Burde ikke være eid av Model, men brukt i rendering av Model.
 */
namespace Gra_desc {

    // TODO opprett UBOMem her automatisk bak descriptor?

    struct DescriptorBindInfo {
        const VkDescriptorType type{};
        const uint32_t bindingNum{};
        const uint32_t count{1};
        const VkShaderStageFlags stageFlags{};
        const std::string &textureName{};
        const uint32_t sizeofUBO{0};
        // const VkDescriptorImageInfo*  pImageInfo;
        // const VkDescriptorBufferInfo* pBufferInfo;
    };

    /*
     * One descriptor describes one resource.
     * Always organized in sets. Can contain one or more descriptors.
     * Can combine descriptors which are usedin conjuction.
     */
    struct DescriptorSet {
        VkDescriptorSet sets[2];
    };

    struct DescriptorBox {
        VkDescriptorPool pool{}; 
        std::vector<DescriptorSet> sets{};
        VkDescriptorSetLayout layout{};
        Gra_Uniform::UBOMem uboMem{};
    };

    // void bindDescriptor(DescriptorSet &descriptor,
    //                     const std::vector<DescriptorBindInfo> &bindInfos,
    //                     const int bufferIndex,
    //                     const int swapIndex,
    //                     const Gra_Uniform::UBOMem &uboMem,
    //                     const std::vector<VkImageView> &texImageViews);

    VkDescriptorSetLayout createDescriptorSetLayout(std::vector<VkDescriptorSetLayoutBinding> bindings);
    VkDescriptorPool createDescriptorPool(int amountEntities);
    DescriptorBox createDescriptorBox(const int amount,
                                      const std::vector<DescriptorBindInfo> &bindInfos);
    void destroyDescriptorBox(const DescriptorBox &box);


}
