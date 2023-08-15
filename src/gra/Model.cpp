//
// Created by jonah on 8/5/2023.
//

#include "Model.h"
#include "vk/shading/gra_uniform.h"
#include "vk/gra_setup.h"
#include "vk/drawing/gra_drawing.h"

std::vector<Model> m_renderModels;

Model::Model() {
    // TODO Alle disse er hardkodet til shaderen triangle mtp bindings og attributes. Feks at de først har uniform buffer og så image sampler.
    descriptorSetLayout = Gra::createDescriptorSetLayout(); // TODO endre her til å binde komponenter senere. ATM er det ubo og 2dsample som er hardkodet.
    pipeline = Raster::createGraphicsPipeline(descriptorSetLayout, "triangle");
    pool = Gra::createDescriptorPool();
    uboMem = Gra::createUniformBuffers();
    descriptorSets = Gra::createDescriptorSets(descriptorSetLayout, pool, uboMem);

    // TODO lag en check på int størrelse i forhold til attributeDescriptions[1].format osv fordi om du definerer feil int størrelse så vil den ikke klage men heller ikke fungere!
    mesh.indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4
    };
    mesh.vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };
    Mesh::init(&mesh);
}

void Model::destroy() {
    mesh.destroy();
    vkDestroyDescriptorPool(Gra::m_device, pool, nullptr);
    Raster::destroyPipeline(pipeline);
    vkDestroyDescriptorSetLayout(Gra::m_device, descriptorSetLayout, nullptr);
}

VkCommandBuffer Model::renderMeshes(uint32_t imageIndex) {
    auto cmd = cmdBuffer.commandBuffers[Drawing::currSwapFrame];
    vkResetCommandBuffer(cmd, 0);
    Gra::recordCommandBuffer(cmd, imageIndex, mesh, pipeline, &descriptorSets[Drawing::currSwapFrame]);
    Gra::updateUniformBuffer(uboMem, Drawing::currSwapFrame, x, 0);
    return cmd;
}

//void Model::drawRenderPass(VkCommandBuffer commandBuffer) {
//    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
//    {
//        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.graphicsPipeline);
//        Gra::recordSwapChain(commandBuffer);
//
//        VkBuffer vertexBuffers[] = {m_vertexBuffer};
//        VkDeviceSize offsets[] = {0};
//        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
//
//        vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
//
//        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipelineLayout, 0, 1, &descriptorSets[Drawing::currSwapFrame], 0, nullptr);
//
//        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
//        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()) - 2, 1, 2, 0, 0);
//    }
//    vkCmdEndRenderPass(commandBuffer);
//}