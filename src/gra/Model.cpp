#include "Model.h"

#include "vk/shading/gra_uniform.h"
#include "vk/gra_setup.h"
#include "vk/drawing/gra_drawing.h"
#include "vk/shading/gra_vertex.h"

std::vector<Model*> m_renderModels;

Model::Model(const std::string& shaderName, const std::string& textureName) {
    // TODO Alle disse er hardkodet til shaderen triangle mtp bindings og attributes. Feks at de først har uniform buffer og så image sampler.
    entities.emplace_back(Entity{});
    descriptorSetLayout = Gra::createDescriptorSetLayout(); // TODO endre her til å binde komponenter senere. ATM er det ubo og 2dsample som er hardkodet.
    pipeline = Raster::createGraphicsPipeline(descriptorSetLayout, shaderName);
    uboMem = Gra::createUniformBuffers(1);
    auto img = Texture::loadImage(textureName.data());
    texImageView = Texture::createTexture(img);
    pool = Gra::createDescriptorPool();
    descriptorSets = Gra::createDescriptorSets(descriptorSetLayout, pool, uboMem, texImageView);

    mesh.init(static_cast<float>(img.w), static_cast<float>(img.h));
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
    auto amount = static_cast<int>(entities.size() + 1);
    if (amount < uboMem.size)
        return;
    uboMem.destroy();
    uboMem = Gra::createUniformBuffers(amount);
    // TODO gjenbruk gamle descriptors...
    descriptorSets = Gra::createDescriptorSets(descriptorSetLayout, pool, uboMem, texImageView);

}

Entity& Model::addEntity(bool update) {
    if (update)
        updateUboBuffer();
    Entity entity{};
    return entities.emplace_back(entity);
}
