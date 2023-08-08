//
// Created by jonah on 8/5/2023.
//

#include "Model.h"
#include "vk/shading/gra_uniform.h"
#include "vk/gra_setup.h"


Model::Model() {
    // TODO Alle disse er hardkodet til shaderen triangle mtp bindings og attributes. Feks at de først har uniform buffer og så image sampler.
    descriptorSetLayout = Gra::createDescriptorSetLayout();
    pipeline = Raster::createGraphicsPipeline(descriptorSetLayout, "triangle");
    descriptorPool = Gra::createDescriptorPool();
}

void Model::destroy() {
    vkDestroyDescriptorPool(Gra::m_device, descriptorPool, nullptr);
    Raster::destroyPipeline(pipeline);
    vkDestroyDescriptorSetLayout(Gra::m_device, descriptorSetLayout, nullptr);
}
