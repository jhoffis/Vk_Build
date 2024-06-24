#pragma once
#include "vk/drawing/gra_command_buffers.h"
#include "vk/drawing/gra_drawing.h"
#include "vk/shading/gra_uniform.h"
#include <vector>
#include <vulkan/vulkan_core.h>

/*
 * Burde ikke være eid av Model, men brukt i rendering av Model.
 */
namespace Gra_desc {

    // TODO opprett UBOMem her automatisk bak descriptor?

    /*
     * One descriptor describes one resource.
     * Always organized in sets. Can contain one or more descriptors.
     * Can combine descriptors which are used in conjuction.
     */
    struct DescriptorSet {
        int bufferIndex{}; // where on the UBOMem buffer 
        VkDescriptorSet sets[Gra::MAX_FRAMES_IN_FLIGHT];
    };

    struct DescriptorBox {
        VkDescriptorPool pool{}; 
        Gra_Uniform::UBOMem uboMem{};
        std::vector<DescriptorSet> sets{};
    };


    void bindUBODescriptor(DescriptorSet &descriptor,
                           const int swapIndex,
                           const Gra_Uniform::UBOMem &uboMem);

    void bindImageDescriptor(DescriptorSet &descriptor,
                             const int swapIndex,
                             const std::vector<VkImageView> &texImageViews);

    VkDescriptorSetLayout createDescriptorSetLayout(std::vector<VkDescriptorSetLayoutBinding> bindings);
    VkDescriptorPool createDescriptorPool(int amountEntities);
    DescriptorBox createDescriptorBox(const int amount);
}
