#include <stdexcept>
#include <iostream>
#include "Model.h"

#include "vk/shading/gra_uniform.h"
#include "vk/gra_setup.h"
#include "vk/drawing/gra_drawing.h"
#include "vk/shading/gra_vertex.h"
#include "src/shaders/SelectionBoxModel.h"
#include "vk/presentation/gra_swap_chain.h"
#include "camera.h"
#include "ShaderSetup.h"

std::vector<Model *> m_renderModels{};
#ifdef RMDEV
std::vector<Raster::Pipeline> m_leftoverPipelines;
#endif


void Model::init(ModelInfo info) {
    // TODO Alle disse er hardkodet til shaderen triangle mtp bindings og attributes. Feks at de først har uniform buffer og så image sampler.
    shaderName = info.shaderName;
    cmdBuffer.init();

    // TODO endre her til å binde komponenter senere. ATM er det ubo og 2dsample som er hardkodet.
    descriptorSetLayout = Gra_Uniform::createDescriptorSetLayout(ShaderSetup::createBindings(info.shaderName));
    createPipeline();

    auto w = info.fallbackWidth;
    auto h = info.fallbackHeight;
    if (info.textureName != nullptr) {
        auto img = Texture::loadImage(info.textureName);  // lik
        texImageView = Texture::createTexture(img);  // lik
        if (w == 0)
            w = static_cast<float>(img.w);
        if (h == 0)
            h = static_cast<float>(img.h);
    }
    mesh.init(w, h);
    uboMem = ShaderSetup::createMem(info.shaderName);
    Gra::createVertexBuffer(&mesh);
    Gra::createIndexBuffer(&mesh);

    pool = Gra_Uniform::createDescriptorPool(1);

    descriptorSets = Gra_Uniform::createDescriptorSets(info.shaderName, descriptorSetLayout, pool, uboMem,
                                                       texImageView);

    m_renderModels.emplace_back(this);
}

VkCommandBuffer Model::renderMeshes(uint32_t imageIndex) {
    auto cmd = cmdBuffer.commandBuffers[Drawing::currSwapFrame];
    vkResetCommandBuffer(cmd, 0);
    Gra::recordCommandBuffer(cmd, imageIndex, mesh, pipeline, descriptorSets);
    for (auto i = 0; i < entities.size(); i++) {
        ShaderSetup::updateRenderUbo(shaderName, entities[i], uboMem);
        Gra_Uniform::updateUniformBuffer(uboMem, Drawing::currSwapFrame, i);
    }
    return cmd;
}

void Model::recreateUboBuffer() {
    // liste med alle referanser til ubos - bare utvid vector listen med descSets og så bruk currswapframe for å tegne alle.
    auto entitiesSize = static_cast<int>(entities.size() + 1);
    if (entitiesSize < uboMem.amount)
        return;
    auto amount = 2 * uboMem.amount;
    while (amount < entitiesSize)
        amount *= 2;
    vkDestroyDescriptorPool(Gra::m_device, pool, nullptr);
    pool = Gra_Uniform::createDescriptorPool(amount);
    uboMem.destroy();
    uboMem = Gra_Uniform::createUniformBuffers(amount, uboMem.range);
    descriptorSets = Gra_Uniform::createDescriptorSets(shaderName, descriptorSetLayout, pool, uboMem, texImageView);
}

void Model::addEntity(std::shared_ptr<Entity> entity, bool update) {
    if (update)
        recreateUboBuffer();
    entities.emplace_back(entity);
}

void Model::createPipeline() {
    pipeline = Raster::createGraphicsPipeline(descriptorSetLayout, ShaderSetup::getShaderName(shaderName));
}

#ifdef RMDEV

void recreateModelPipelines() {
    for (auto model: m_renderModels) {
        m_leftoverPipelines.emplace_back(model->pipeline);
        model->createPipeline();
    }
}

#endif


void Model::destroy() {
    vkDestroyCommandPool(Gra::m_device, cmdBuffer.commandPool, nullptr);
    uboMem.destroy();
    vkDestroyDescriptorPool(Gra::m_device, pool, nullptr);
    Raster::destroyPipeline(pipeline);
    vkDestroyDescriptorSetLayout(Gra::m_device, descriptorSetLayout, nullptr);
}


void destroyModels() {
    for (auto model: m_renderModels) {
        model->destroy();
    }

#ifdef RMDEV
    for (auto pipeline: m_leftoverPipelines) {
        Raster::destroyPipeline(pipeline);
    }
#endif
}
