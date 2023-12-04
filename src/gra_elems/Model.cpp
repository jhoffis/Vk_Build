#include "Model.h"

#include "vk/shading/gra_uniform.h"
#include "vk/gra_setup.h"
#include "vk/drawing/gra_drawing.h"
#include "vk/shading/gra_vertex.h"

std::vector<Model*> m_renderModels;

void Model::init(ModelInfo modelInfo) {
    // TODO Alle disse er hardkodet til shaderen triangle mtp bindings og attributes. Feks at de først har uniform buffer og så image sampler.
    cmdBuffer.init();

    descriptorSetLayout = Gra::createDescriptorSetLayout(modelInfo.bindings); // TODO endre her til å binde komponenter senere. ATM er det ubo og 2dsample som er hardkodet.
    pipeline = Raster::createGraphicsPipeline(descriptorSetLayout, modelInfo.shaderName);
    uboMem = Gra::createUniformBuffers(1, modelInfo.sizeOfUBO); // sizeof(Gra::UniformBufferObject));

    if (modelInfo.textureName != nullptr) {
        auto img = Texture::loadImage(modelInfo.textureName);
        texImageView = Texture::createTexture(img);
        if (modelInfo.fallbackWidth == 0)
            modelInfo.fallbackWidth = static_cast<float>(img.w);
        if (modelInfo.fallbackHeight == 0)
            modelInfo.fallbackHeight = static_cast<float>(img.h);
    }
    pool = Gra::createDescriptorPool(1);
    descriptorSets = Gra::createDescriptorSets(descriptorSetLayout, pool, uboMem, texImageView);

    mesh.init(modelInfo.fallbackWidth, modelInfo.fallbackHeight);
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

void Model::updateUboBuffer() {
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
    descriptorSets = Gra::createDescriptorSets(descriptorSetLayout, pool, uboMem, texImageView);
}

void Model::addEntity(Entity* entity, bool update) {
    if (update)
        updateUboBuffer();
    entities.emplace_back(entity);
}
