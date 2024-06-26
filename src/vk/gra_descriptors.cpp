#include "gra_descriptors.h"
#include "vk/gra_setup.h"
#include <array>
#include <cassert>
#include <iostream>

namespace Gra_desc {

    VkDescriptorSetLayoutBinding layoutBinding(const DescriptorBindInfo &bindInfo) {
        return {
                .binding = bindInfo.bindingNum, // binding is the binding number of this entry and corresponds to a resource of the same binding number in the shader stages.
                .descriptorType = bindInfo.type,
                .descriptorCount = bindInfo.count,
                .stageFlags = bindInfo.stageFlags,
                .pImmutableSamplers = nullptr // only relevant for image sampling related descriptor,
        };
    }
 

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


    void bindDescriptor(DescriptorSet &descriptor,
                        const std::vector<DescriptorBindInfo> &bindInfos,
                        const int bufferIndex,
                        const Gra_Uniform::UBOMem &uboMem,
                        const std::vector<VkImageView> &texImageViews) {
        // setup phase
        std::vector<VkDescriptorImageInfo> imageInfos{texImageViews.size()};
        for (int i = 0; i < imageInfos.size(); i++) {
            imageInfos[i] = VkDescriptorImageInfo{
                .sampler = Texture::m_textureSampler,
                .imageView = texImageViews[i],
                .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            };
        }
        std::cout << bindInfos[0].type << std::endl;
        for (int swapIndex = 0; swapIndex < 2; swapIndex++) { 
            std::vector<VkWriteDescriptorSet> descriptorWrites{bindInfos.size()};

            int nImg = 0; // gjør dette for hver entity basically.
            for (auto i = 0; i < descriptorWrites.size(); i++) {
                descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[i].dstSet = descriptor.sets[swapIndex];
                descriptorWrites[i].dstBinding = bindInfos[i].bindingNum;
                descriptorWrites[i].dstArrayElement = 0;
                descriptorWrites[i].descriptorType = bindInfos[i].type;
                descriptorWrites[i].descriptorCount = bindInfos[i].count;
                if (bindInfos[i].type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) { // FIXME
                    VkDescriptorBufferInfo bufferInfo{};
                    bufferInfo.buffer = uboMem.uniformBuffers[swapIndex]; 
                    bufferInfo.offset = uboMem.offset * bufferIndex;
                    bufferInfo.range  = uboMem.range;
                    descriptorWrites[i].pBufferInfo = &bufferInfo;
                } else if (bindInfos[i].type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
                    descriptorWrites[i].pImageInfo = &imageInfos[nImg];
                    nImg++;
                }
            }

            vkUpdateDescriptorSets(Gra::m_device,
                    static_cast<uint32_t>(descriptorWrites.size()),
                    descriptorWrites.data(),
                    0, 
                    nullptr);
        }
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


    VkDescriptorPool createDescriptorPool2(const int amount,
                                           const std::vector<DescriptorBindInfo> &bindInfos) {
        std::vector<VkDescriptorPoolSize> poolSizes{bindInfos.size()};
        for (int i = 0; i < bindInfos.size(); i++) {
            poolSizes[i] = VkDescriptorPoolSize{
                .type = bindInfos[i].type,
                .descriptorCount = amount * bindInfos[i].count
            };
        }

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

    DescriptorBox createDescriptorBox(const int amount,
                                      const std::vector<DescriptorBindInfo> &bindInfos) {
        assert(amount > 0);
        auto box = DescriptorBox{};
        auto amountWithSwap = 2 * amount; // Dette er amount: auto amount =
                                                                  // MyMath::nextPowerOfTwo(entitiesSize); Men...
                                                                  // why? Hvorfor ikke bare ha mengden entities?
                                                                  // Fordi da må man kjøre denne for hver nye entity
                                                                  // som lages, men man sparer mange fps av det da...
        box.pool = createDescriptorPool2(amountWithSwap, bindInfos);
        std::vector<VkDescriptorSetLayoutBinding> bindings{bindInfos.size()};
        for (int i = 0; i < bindInfos.size(); i++) {
            bindings[i] = layoutBinding(bindInfos[i]);
        }
        box.layout = createDescriptorSetLayout(bindings);
        std::vector<VkDescriptorSetLayout> layouts(amountWithSwap, box.layout);

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = box.pool;
        allocInfo.descriptorSetCount = amountWithSwap;
        allocInfo.pSetLayouts = layouts.data();

        std::vector<VkDescriptorSet> descriptorSets{};
        descriptorSets.resize(amountWithSwap);
        auto res = vkAllocateDescriptorSets(Gra::m_device, 
                                            &allocInfo,
                                            descriptorSets.data());
        if (res != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        box.sets.resize(amount);
        for (int i = 0; i < amountWithSwap; i += 2) {
            box.sets[i / 2] = {.sets={descriptorSets[i], descriptorSets[i+1]}};            
        }

        box.uboMem = Gra_Uniform::createUniformBuffers(amount, bindInfos[0].sizeofUBO); // FIXME
                                                                                        
        std::vector<std::string> texs{};
        for (int i = 0; i < bindInfos.size(); i++) {
            if (bindInfos[i].textureName.size() > 0) {
                texs.emplace_back(bindInfos[i].textureName);
            }
        }
        auto images = Texture::getTexImageViews(texs);                             

        for (int i = 0; i < box.sets.size(); i++) {
            bindDescriptor(
                box.sets[i],
                bindInfos,
                i,
                box.uboMem,
                images
            );
        }

        return box;
    }

    void destroyDescriptorBox(const DescriptorBox &box) {
        box.uboMem.destroy();
        vkDestroyDescriptorPool(Gra::m_device, box.pool, nullptr);
        vkDestroyDescriptorSetLayout(Gra::m_device, box.layout, nullptr);
    }
}




