//
// Created by jonah on 8/5/2023.
//

#include "Model.h"
#include "vk/shading/gra_uniform.h"
#include "vk/pipeline/gra_pipeline.h"
#include "vk/gra_setup.h"


Model::Model() {
    descriptorSetLayout = Gra::createDescriptorSetLayout();
    graphicsPipeline = Gra::createGraphicsPipeline(descriptorSetLayout); // TODO hent lagre og slett pipelinelayouten også
}

void Model::destroy() {
    vkDestroyPipeline(Gra::m_device, graphicsPipeline, nullptr);
    vkDestroyDescriptorSetLayout(Gra::m_device, descriptorSetLayout, nullptr);
}
