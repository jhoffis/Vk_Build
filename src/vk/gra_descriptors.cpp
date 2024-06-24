#include "gra_descriptors.h"
#include "vk/drawing/gra_command_buffers.h"
#include "vk/gra_setup.h"
#include <array>
#include <cassert>
namespace Gra_desc {


    VkDescriptorSetLayoutBinding uboLayoutBinding(uint32_t bindingNum) {
        return {
                .binding = bindingNum, // binding is the binding number of this entry and corresponds to a resource of the same binding number in the shader stages.
                .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .descriptorCount = 1,
                .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
                .pImmutableSamplers = nullptr // only relevant for image sampling related descriptor,
        };
    }
    VkDescriptorSetLayoutBinding imageLayoutBinding(uint32_t bindingNum) {
        return {
                .binding = bindingNum,
                .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .descriptorCount = 1,
                .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT, // fragment shader.  It is possible to use texture sampling in the vertex shader, for example to dynamically deform a grid of vertices by a heightmap
                .pImmutableSamplers = nullptr,
        };
    }


    void bindUBODescriptor(DescriptorSet &descriptor,
                           const int swapIndex,
                           const Gra_Uniform::UBOMem &uboMem) {
        assert(swapIndex >= 0 && swapIndex <= Gra::MAX_FRAMES_IN_FLIGHT);
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uboMem.uniformBuffers[swapIndex]; 
        bufferInfo.offset = uboMem.offset * descriptor.bufferIndex;
        bufferInfo.range  = uboMem.range;

        std::vector<VkWriteDescriptorSet> descriptorWrites{1}; // TODO be able to bind more than one ubo to a descriptor

        for (auto i = 0; i < descriptorWrites.size(); i++) {
            descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[i].dstSet = descriptor.sets[swapIndex];
            descriptorWrites[i].dstBinding = i;
            descriptorWrites[i].dstArrayElement = 0;
            descriptorWrites[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[i].descriptorCount = 1;
            descriptorWrites[i].pBufferInfo = &bufferInfo;
        }

        vkUpdateDescriptorSets(Gra::m_device,
                               static_cast<uint32_t>(descriptorWrites.size()),
                               descriptorWrites.data(), 0, nullptr);
    }

    void bindImageDescriptor(DescriptorSet &descriptor,
                               const int swapIndex,
                               const std::vector<VkImageView> &texImageViews) {
        assert(swapIndex >= 0 && swapIndex <= Gra::MAX_FRAMES_IN_FLIGHT);
        // setup phase
        std::vector<VkDescriptorImageInfo> imageInfos{texImageViews.size()};
        for (int i = 0; i < imageInfos.size(); i++) {
            imageInfos[i] = VkDescriptorImageInfo{
                .sampler = Texture::m_textureSampler,
                .imageView = texImageViews[i],
                .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            };
        }

        std::vector<VkWriteDescriptorSet> descriptorWrites{1};

        int nImg = 0; // gjør dette for hver entity basically.
        for (auto i = 0; i < descriptorWrites.size(); i++) {
            descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[i].dstSet = descriptor.sets[swapIndex];
            descriptorWrites[i].dstBinding = i;
            descriptorWrites[i].dstArrayElement = 0;
            descriptorWrites[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[i].descriptorCount = 1;
            descriptorWrites[i].pImageInfo = &imageInfos[nImg];
            nImg++;
        }

        // IT SAYS UPDATE. Can I run update on a singular desciptor without having
        // to delete and recreate everything?
        vkUpdateDescriptorSets(Gra::m_device,
                               static_cast<uint32_t>(descriptorWrites.size()),
                               descriptorWrites.data(), 0, nullptr);
    }

    VkDescriptorSetLayout createDescriptorSetLayout(std::vector<VkDescriptorSetLayoutBinding> bindings) {
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        VkDescriptorSetLayout descriptorSetLayout{};
        if (vkCreateDescriptorSetLayout(Gra::m_device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
        return descriptorSetLayout;
    }

    VkDescriptorPool createDescriptorPool(int amount) {
        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = amount; 
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = amount;

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = poolSizes.size();
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = amount + 1;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

        VkDescriptorPool pool{};
        if (vkCreateDescriptorPool(Gra::m_device, &poolInfo, nullptr, &pool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }
        return pool;
    }

    DescriptorBox createDescriptorBox(const int amount) {
        assert(amount > 0);
        auto box = DescriptorBox{};
        auto size = Gra::MAX_FRAMES_IN_FLIGHT * amount; // Dette er amount: auto amount =
                                                        // MyMath::nextPowerOfTwo(entitiesSize); Men...
                                                        // why? Hvorfor ikke bare ha mengden entities?
                                                        // Fordi da må man kjøre denne for hver nye entity
                                                        // som lages, men man sparer mange fps av det da...
        box.pool = Gra_desc::createDescriptorPool(size);
        auto descriptorSetLayout = createDescriptorSetLayout({uboLayoutBinding(0),
                                                              imageLayoutBinding(1)});
        std::vector<VkDescriptorSetLayout> layouts(size, descriptorSetLayout);

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = box.pool;
        allocInfo.descriptorSetCount = size;
        allocInfo.pSetLayouts = layouts.data();

        std::vector<VkDescriptorSet> descriptorSets{};
        descriptorSets.resize(size);
        auto res = vkAllocateDescriptorSets(Gra::m_device, 
                                            &allocInfo,
                                            descriptorSets.data()); // is this deleted ever? Yes, via pool
        if (res != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        std::vector<VkImageView> texImageViews{};

        // for (auto i = 0; i < uboMem.amount; i++) {
        //     // TODO here we can change the imageInfo to the one you want
        //     // because the var size is the amount of entities!!!
        //     if (order.size() == 2) {
        //         Drawing::currSwapFrame = 0;
        //         updateDescriptorSet(i, texImageViews2, uboMem, order, descriptorSets);
        //         Drawing::currSwapFrame = 1;
        //         updateDescriptorSet(i, texImageViews2, uboMem, order, descriptorSets);
        //     } else {
        //         Drawing::currSwapFrame = 0;
        //         updateDescriptorSet(i, texImageViews, uboMem, order, descriptorSets);
        //         Drawing::currSwapFrame = 1;
        //         updateDescriptorSet(i, texImageViews, uboMem, order, descriptorSets);
        //     }
        // }
        //
        // Drawing::currSwapFrame = ogSwapFrame;
        return box;

    }
}



