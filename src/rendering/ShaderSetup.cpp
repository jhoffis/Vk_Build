
#include <stdexcept>
#include "ShaderSetup.h"
#include "vk/drawing/gra_command_buffers.h"
#include "vk/shading/gra_vertex.h"
#include "shaders/SelectionBoxModel.h"
#include "camera.h"
#include "vk/presentation/gra_swap_chain.h"

// TODO handle the in aspects of the shaders.
namespace ShaderSetup {

    struct ComponentUBO {

        // sizeofUBO

//        {
//            .binding = 0,
//            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
//            .descriptorCount = 1,
//            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
//            .pImmutableSamplers = nullptr // only relevant for image sampling related descriptor,
//        },
//
//        VkDescriptorBufferInfo bufferInfo{};
//        bufferInfo.buffer = uboMem.uniformBuffers[i % Gra::MAX_FRAMES_IN_FLIGHT]; // TODO Her er bindingen til ubo o.l.
//        bufferInfo.offset = uboMem.offset * static_cast<int>(
//                floor(static_cast<float>(i) / static_cast<float>(Gra::MAX_FRAMES_IN_FLIGHT)));
//        bufferInfo.range = uboMem.range;
//
//        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};
//
//        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//        descriptorWrites[0].dstSet = descriptorSets[i];
//        descriptorWrites[0].dstBinding = 0;
//        descriptorWrites[0].dstArrayElement = 0;
//        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//        descriptorWrites[0].descriptorCount = 1;
//        descriptorWrites[0].pBufferInfo = &bufferInfo;
//
//        vkUpdateDescriptorSets(Gra::m_device,
//        static_cast<uint32_t>(descriptorWrites.size()),
//        descriptorWrites.data(),
//        0,
//        nullptr);
    };

    struct ComponentImage {
//        {
//            .binding = 1,
//            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
//            .descriptorCount = 1,
//            .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT, // fragment shader.  It is possible to use texture sampling in the vertex shader, for example to dynamically deform a grid of vertices by a heightmap
//            .pImmutableSamplers = nullptr,
//        }
//
//        VkDescriptorImageInfo imageInfo{};
//        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//        imageInfo.imageView = textureImageView;
//        imageInfo.sampler = Texture::m_textureSampler;
//
//        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//        descriptorWrites[1].dstSet = descriptorSets[i];
//        descriptorWrites[1].dstBinding = 1;
//        descriptorWrites[1].dstArrayElement = 0;
//        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//        descriptorWrites[1].descriptorCount = 1;
//        descriptorWrites[1].pImageInfo = &imageInfo;
    };

	// Yes. This is related to the filename
    std::string getShaderName(const ShaderName &shader) {
        switch (shader) {
            case grass:
                return "grass";
            case selectionBox:
                return "selectionbox";
            default:
                throw std::invalid_argument("Could not create shader-name");
        }
    }

	// Yes. This is related to the shader-file's UBO. Could later be expanded to include more uniforms.
	// Could it be expanded to be able to change the contents of  a sampler2D also?
	// No repeat. Different for each?
    Gra_Uniform::UBOMem createMem(const ShaderName &shader) {
        switch (shader) {
            case grass:
                return Gra_Uniform::createUniformBuffers(1, sizeof(Gra::UniformBufferObject));
            case selectionBox:
                return Gra_Uniform::createUniformBuffers(1, sizeof(SelectionBox::SelectionBoxUBO));
            default :
                throw std::invalid_argument("Could not create shader-bindings");
        }
    }

	// Yes. This is related to filling the shader-file's UBO.
	// Virtually no repeat. Unique.
    void updateRenderUbo(const ShaderName &shaderName,
                         const std::shared_ptr<Entity> &entity,
                         Gra_Uniform::UBOMem &uboMem) {
        switch (shaderName) {
            case grass: {
                auto pos = entity->pos;

                delete static_cast<Gra::UniformBufferObject*>(uboMem.uboStruct);
                uboMem.uboStruct = new Gra::UniformBufferObject{
                        .pos = pos - Camera::m_cam.pos,
                        .aspect = Gra::m_swapChainAspectRatio,
                };
                break;
            }
            case selectionBox: {
                SelectionBox::m_ubo.aspect = Gra::m_swapChainAspectRatio;
                SelectionBox::m_ubo.resolution.x = Window::WIDTH; // kanskje monitor size istedet?
                SelectionBox::m_ubo.resolution.y = Window::HEIGHT;
                SelectionBox::m_ubo.posCam.x = Camera::m_cam.pos.x;
                SelectionBox::m_ubo.posCam.y = Camera::m_cam.pos.y;
                uboMem.uboStruct = &SelectionBox::m_ubo;
                break;
            }
        }
    }

	// Yes. Hm, but where is the image added to the fragment? This is the connector wire between code and shader-file.
	// Lot's of repeat.
    std::vector<VkDescriptorSetLayoutBinding> createBindings(const ShaderName &shader) {
        switch (shader) {
            case grass:
                return {
                        UBOComponent::binding(0),
                        ImageComponent::binding(1)
                };
            case selectionBox:
                return {
                        UBOComponent::binding(0)
                };
            default :
                throw std::invalid_argument("Could not create shader-bindings");
        }
    }

	// Yes. UBO is filled above. Creates buffers and initializes uniforms.
	// The loop and the adding of UBO is the exact same for both, except grass has 2 with image.
    void shaderFillDescriptorSets(const ShaderName &shader,
                                  const Gra_Uniform::UBOMem &uboMem,
                                  const VkImageView &textureImageView,
                                  int size,
                                  const std::vector<VkDescriptorSet> &descriptorSets) {
        switch (shader) {
            case grass: {
                VkDescriptorImageInfo imageInfo{};
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = textureImageView;
                imageInfo.sampler = Texture::m_textureSampler;

                for (auto i = 0; i < size; i++) {

                    VkDescriptorBufferInfo bufferInfo{};
                    bufferInfo.buffer = uboMem.uniformBuffers[i %
                                                              Gra::MAX_FRAMES_IN_FLIGHT]; // TODO Her er bindingen til ubo o.l.
                    bufferInfo.offset = uboMem.offset * static_cast<int>(
                           floor(static_cast<float>(i) / static_cast<float>(Gra::MAX_FRAMES_IN_FLIGHT)));
                    bufferInfo.range = uboMem.range;

                    std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

                    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    descriptorWrites[0].dstSet = descriptorSets[i];
                    descriptorWrites[0].dstBinding = 0;
                    descriptorWrites[0].dstArrayElement = 0;
                    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    descriptorWrites[0].descriptorCount = 1;
                    descriptorWrites[0].pBufferInfo = &bufferInfo;

                    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    descriptorWrites[1].dstSet = descriptorSets[i];
                    descriptorWrites[1].dstBinding = 1;
                    descriptorWrites[1].dstArrayElement = 0;
                    descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    descriptorWrites[1].descriptorCount = 1;
                    descriptorWrites[1].pImageInfo = &imageInfo;

                    vkUpdateDescriptorSets(Gra::m_device,
                                           static_cast<uint32_t>(descriptorWrites.size()),
                                           descriptorWrites.data(),
                                           0,
                                           nullptr);
                }
                return;
            }
            case selectionBox: {

                for (auto i = 0; i < size; i++) {

                    VkDescriptorBufferInfo bufferInfo{};
                    bufferInfo.buffer = uboMem.uniformBuffers[i % Gra::MAX_FRAMES_IN_FLIGHT]; // TODO Her er bindingen til ubo o.l.
                    bufferInfo.offset = uboMem.offset * static_cast<int>(
                            floor(static_cast<float>(i) / static_cast<float>(Gra::MAX_FRAMES_IN_FLIGHT)));
                    bufferInfo.range = uboMem.range;

                    std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

                    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    descriptorWrites[0].dstSet = descriptorSets[i];
                    descriptorWrites[0].dstBinding = 0;
                    descriptorWrites[0].dstArrayElement = 0;
                    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    descriptorWrites[0].descriptorCount = 1;
                    descriptorWrites[0].pBufferInfo = &bufferInfo;

                    vkUpdateDescriptorSets(Gra::m_device,
                                           static_cast<uint32_t>(descriptorWrites.size()),
                                           descriptorWrites.data(),
                                           0,
                                           nullptr);
                }
                return;
            }
        }
        throw std::invalid_argument("Could not fill shader descriptor-sets");
    }

} // Shader
