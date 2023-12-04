#include <stdexcept>
#include "Model.h"

#include "vk/shading/gra_uniform.h"
#include "vk/gra_setup.h"
#include "vk/drawing/gra_drawing.h"
#include "vk/shading/gra_vertex.h"

std::vector<Model*> m_renderModels;

std::vector<VkDescriptorSetLayoutBinding> createBindings(const ShaderName &shader) {
    if (shader == triangle) {
        return {
                {
                        .binding = 0,
                        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                        .descriptorCount = 1,
                        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
                        .pImmutableSamplers = nullptr // only relevant for image sampling related descriptor,
                },
                {
                        .binding = 1,
                        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                        .descriptorCount = 1,
                        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT, // fragment shader.  It is possible to use texture sampling in the vertex shader, for example to dynamically deform a grid of vertices by a heightmap
                        .pImmutableSamplers = nullptr,
                }
        };
    }
    throw std::invalid_argument("Could not create shader-bindings");
}

std::string createShaderName(const ShaderName &shader) {
    switch (shader)
    {
        case triangle:   return "triangle";
        default:      throw std::invalid_argument("Could not create shader-name");
    }
}


void Model::init(ModelInfo info) {
    // TODO Alle disse er hardkodet til shaderen triangle mtp bindings og attributes. Feks at de først har uniform buffer og så image sampler.
    shaderName = info.shaderName;
    cmdBuffer.init();

    descriptorSetLayout = Gra::createDescriptorSetLayout(createBindings(info.shaderName)); // TODO endre her til å binde komponenter senere. ATM er det ubo og 2dsample som er hardkodet.
    pipeline = Raster::createGraphicsPipeline(descriptorSetLayout, createShaderName(info.shaderName));
    if (info.shaderName == triangle) {
        uboMem = Gra::createUniformBuffers(1, sizeof(Gra::UniformBufferObject));
    }

    auto w = info.fallbackWidth;
    auto h = info.fallbackHeight;

    if (info.textureName != nullptr) {
        auto img = Texture::loadImage(info.textureName);
        texImageView = Texture::createTexture(img);
        if (w == 0)
            w = static_cast<float>(img.w);
        if (h == 0)
            h = static_cast<float>(img.h);
    }
    pool = Gra::createDescriptorPool(1);
    descriptorSets = Gra::createDescriptorSets(info.shaderName, descriptorSetLayout, pool, uboMem, texImageView);

    mesh.init(w, h);
    Gra::createVertexBuffer(&mesh);
    Gra::createIndexBuffer(&mesh);
}

void Model::destroy() {
    vkDestroyCommandPool(Gra::m_device, cmdBuffer.commandPool, nullptr);
    uboMem.destroy();
    vkDestroyDescriptorPool(Gra::m_device, pool, nullptr);
    Raster::destroyPipeline(pipeline);
    vkDestroyDescriptorSetLayout(Gra::m_device, descriptorSetLayout, nullptr);
}

VkCommandBuffer Model::renderMeshes(uint32_t imageIndex) {
    auto cmd = cmdBuffer.commandBuffers[Drawing::currSwapFrame];
    vkResetCommandBuffer(cmd, 0);
    Gra::recordCommandBuffer(cmd, imageIndex, mesh, pipeline, descriptorSets);
    for (auto i = 0; i < entities.size(); i++) {
        Gra::updateUniformBuffer(uboMem, Drawing::currSwapFrame, i, entities[i]);
    }
    return cmd;
}

void Model::recreateUboBuffer() {
    // liste med alle referanser til ubos - bare utvid vector listen med descSets og så bruk currswapframe for å tegne alle.
    auto entitiesSize = static_cast<int>(entities.size() + 1);
    if (entitiesSize < uboMem.amount)
        return;
    auto amount = 2*uboMem.amount;
    while (amount < entitiesSize)
        amount *= 2;
    vkDestroyDescriptorPool(Gra::m_device, pool, nullptr);
    pool = Gra::createDescriptorPool(amount);
    uboMem.destroy();
    uboMem = Gra::createUniformBuffers(amount, uboMem.range);
    descriptorSets = Gra::createDescriptorSets(shaderName, descriptorSetLayout, pool, uboMem, texImageView);
}

void Model::addEntity(Entity* entity, bool update) {
    if (update)
        recreateUboBuffer();
    entities.emplace_back(entity);
}
