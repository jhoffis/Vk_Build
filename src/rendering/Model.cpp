#include <stdexcept>
#include <iostream>
#include <utility>
#include "Model.h"

#include "vk/shading/gra_uniform.h"
#include "vk/gra_setup.h"
#include "vk/drawing/gra_drawing.h"
#include "vk/shading/gra_vertex.h"
#include "models/SelectionBoxModel.h"
#include "vk/presentation/gra_swap_chain.h"
#include "camera.h"
#include "timer_util.h"

void grassUpdateRenderUbo(Gra_Uniform::UBOMem *uboMem,
                          const std::shared_ptr<Entity> &entity) {
    delete static_cast<Gra::UniformBufferObject *>(uboMem->uboStruct);
    uboMem->uboStruct = new Gra::UniformBufferObject{
            .pos = entity->pos - Camera::m_cam.pos,
            .aspect = Gra::m_swapChainAspectRatio,
            .selected = entity->selected,
    };
}

std::vector<Model *> m_renderModels{};
Model Shaders::m_grassModel(
        "grass",
        grassUpdateRenderUbo,
        {
                vert_ubo,
                frag_image,
        },
        sizeof(Gra::UniformBufferObject),
        0, 0,
        {
                "grass.png"
        }
);
Model Shaders::m_houseModel(
        "grass",
        grassUpdateRenderUbo,
        {
                vert_ubo,
                frag_image,
        },
        sizeof(Gra::UniformBufferObject),
        0, 0,
        {
                "house.png"
        }
);

Model Shaders::m_villModel{
        "vill",
        grassUpdateRenderUbo,
        {
                vert_ubo,
                frag_image,
        },
        sizeof(Gra::UniformBufferObject),
        0, 0,
        {
                "unit.png"
        }
};

Model Shaders::m_selectionBoxModel{
        "selectionbox",
        [](auto uboMem, auto entity) {
            SelectionBox::m_ubo.aspect = Gra::m_swapChainAspectRatio;
            SelectionBox::m_ubo.resolution.x = Window::WIDTH; // kanskje monitor size istedet?
            SelectionBox::m_ubo.resolution.y = Window::HEIGHT;
            SelectionBox::m_ubo.posCam.x = Camera::m_cam.pos.x;
            SelectionBox::m_ubo.posCam.y = Camera::m_cam.pos.y;
            uboMem->uboStruct = &SelectionBox::m_ubo;
        }, {
                vert_ubo,
        },
        sizeof(SelectionBox::SelectionBoxUBO),
        128, 128,
        {}
};


#ifdef RMDEV
std::vector<Raster::Pipeline> m_leftoverPipelines;
#endif

std::vector<VkDescriptorSet> Model::createDescriptorSets() const {
    auto size = Gra::MAX_FRAMES_IN_FLIGHT * uboMem.amount;
    std::vector<VkDescriptorSetLayout> layouts(size, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(size);
    allocInfo.pSetLayouts = layouts.data();

    std::vector<VkDescriptorSet> descriptorSets{};
    descriptorSets.resize(size);
    auto res = vkAllocateDescriptorSets(Gra::m_device, &allocInfo, descriptorSets.data());
    if (res != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    // setup phase
    std::vector<VkDescriptorImageInfo> imageInfos{texImageViews.size()};
    for (int i = 0; i < imageInfos.size(); i++) {
        imageInfos[i] = VkDescriptorImageInfo{
                .sampler = Texture::m_textureSampler,
                .imageView = texImageViews[i],
                .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        };
    }

    for (auto i = 0; i < size; i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uboMem.uniformBuffers[i % Gra::MAX_FRAMES_IN_FLIGHT]; // TODO Her er bindingen til ubo o.l.
        bufferInfo.offset = uboMem.offset * static_cast<int>(std::floor(
                static_cast<float>(i) / static_cast<float>(Gra::MAX_FRAMES_IN_FLIGHT)));
        bufferInfo.range = uboMem.range;

        std::vector<VkWriteDescriptorSet> descriptorWrites{static_cast<size_t>(order.size())};

        int nImg = 0;
        for (auto a = 0; a < order.size(); a++) {
            if (order[a] == vert_ubo) {
                descriptorWrites[a].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[a].dstSet = descriptorSets[i];
                descriptorWrites[a].dstBinding = a;
                descriptorWrites[a].dstArrayElement = 0;
                descriptorWrites[a].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptorWrites[a].descriptorCount = 1;
                descriptorWrites[a].pBufferInfo = &bufferInfo;
            } else if (order[a] == frag_image) {
                descriptorWrites[a].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[a].dstSet = descriptorSets[i];
                descriptorWrites[a].dstBinding = a;
                descriptorWrites[a].dstArrayElement = 0;
                descriptorWrites[a].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                descriptorWrites[a].descriptorCount = 1;
                descriptorWrites[a].pImageInfo = &imageInfos[nImg];
                nImg++;
            }
        }

        vkUpdateDescriptorSets(Gra::m_device,
                               static_cast<uint32_t>(descriptorWrites.size()),
                               descriptorWrites.data(),
                               0,
                               nullptr);
    }

    return descriptorSets;
}

Model::Model(std::string shaderName,
             std::function<void(Gra_Uniform::UBOMem *, const std::shared_ptr<Entity> &entity)> updateRenderUbo,
             std::vector<ShaderComponentOrder> order,
             const int sizeOfUBO,
             const float overrideWidth,
             const float overrideHeight,
             std::vector<std::string> textures)
        : shaderName(std::move(shaderName)),
          updateRenderUbo(std::move(updateRenderUbo)),
          order(std::move(order)),
          sizeOfUBO(sizeOfUBO),
          overrideWidth(overrideWidth),
          overrideHeight(overrideHeight),
          textures(std::move(textures)) {
    m_renderModels.emplace_back(this);
}

void Model::init() {
    // TODO Alle disse er hardkodet til shaderen triangle mtp bindings og attributes. Feks at de først har uniform buffer og så image sampler.
    cmdBuffer.init();

    pool = Gra_Uniform::createDescriptorPool(1);

    { // Shader stuff

        auto w = overrideWidth;
        auto h = overrideHeight;
        for (const auto &tex: textures) {
            auto img = Texture::loadImage(tex.c_str());
            texImageViews.emplace_back(Texture::createTexture(img));
            if (w == 0) w = static_cast<float>(img.w);
            if (h == 0) h = static_cast<float>(img.h);
        }
        mesh.init(w, h);
        Gra::createVertexBuffer(&mesh);
        Gra::createIndexBuffer(&mesh);


        uboMem = Gra_Uniform::createUniformBuffers(1, sizeOfUBO);
        // These two can be combined
        std::vector<VkDescriptorSetLayoutBinding> bindings{};
        for (int i = 0; i < order.size(); i++) {
            switch (order[i]) {
                case vert_ubo:
                    bindings.emplace_back(UBOComponent::binding(i));
                    break;
                case frag_image:
                    bindings.emplace_back(ImageComponent::binding(i));
                    break;
            }
        }
        descriptorSetLayout = Gra_Uniform::createDescriptorSetLayout(bindings);
        descriptorSets = createDescriptorSets();
    }
    createPipeline();
}

void Model::runRecreateUbo() {
    // liste med alle referanser til ubos - bare utvid vector listen med descSets og så bruk currswapframe for å tegne alle.
    auto entitiesSize = static_cast<int>(entities.size());
    auto amount = MyMath::nextPowerOfTwo(entitiesSize);
    if (amount == uboMem.amount) return;
    std::cout << "recreates ubo to size " << amount << std::endl;

    auto t0 = Timer::nowNanos();
    vkDeviceWaitIdle(Gra::m_device); // TODO maybe replace this
    auto t1 = Timer::nowNanos();
    vkDestroyDescriptorPool(Gra::m_device, pool, nullptr);
    pool = Gra_Uniform::createDescriptorPool(amount);
    uboMem.destroy();
    uboMem = Gra_Uniform::createUniformBuffers(amount, uboMem.range);
    descriptorSets = createDescriptorSets();
    auto t2 = Timer::nowNanos();
    Timer::printTimeDiffNanos(t0, t1);
    Timer::printTimeDiffNanos(t0, t2);
}

VkCommandBuffer Model::renderMeshes(uint32_t imageIndex) {
    if (queueRecreateUboBuffer) {
        queueRecreateUboBuffer = false;
        runRecreateUbo();
    }

    auto cmd = cmdBuffer.commandBuffers[Drawing::currSwapFrame];
    vkResetCommandBuffer(cmd, 0);
    Gra::recordCommandBuffer(cmd,
                             imageIndex,
                             mesh,
                             pipeline,
                             descriptorSets);
    auto n = 0;
    for (auto i = 0; i < entities.size(); i++) {
        if (!entities[i]->visible)
            continue;
        updateRenderUbo(&uboMem, entities[i]); // TODO maybe just return a ubostruct?
        Gra_Uniform::updateUniformBuffer(uboMem,
                                         Drawing::currSwapFrame,
                                         n);
        // TODO update other uniforms here like imgs
        n++;
    }

    Gra_Uniform::clearUniformBuffer(uboMem,
                                    Drawing::currSwapFrame,
                                    n);

    return cmd;
}

void Model::recreateUboBuffer() {
    queueRecreateUboBuffer = true;
}

void Model::addEntity(const std::shared_ptr<Entity> &entity, bool update) {
    entities.emplace_back(entity);
    if (update)
        recreateUboBuffer();
}

void Model::removeEntity(const std::shared_ptr<Entity> &sharedPtr) {
    entities.erase(std::remove(entities.begin(), entities.end(), sharedPtr), entities.end());
    recreateUboBuffer();
}

void Model::spawn(float x, float y) {
    recreateUboBuffer();
    entities.emplace_back(
            std::make_shared<Entity>(
                    Entity{.pos = {x, y, 0},
                            .size = {width(), height()}}
            )
    );
}

void Model::createPipeline() {
    pipeline = Raster::createGraphicsPipeline(descriptorSetLayout, shaderName);
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

void Model::sort() {
    std::sort(entities.begin(), entities.end(),
              [](auto a, auto b) {
                  return a->pos.y > b->pos.y;
              });
}

void Model::setTextures(const std::vector<std::string> &textures) {
    texImageViews.clear();
    for (const auto &tex: textures) {
        auto img = Texture::loadImage(tex.c_str());
        texImageViews.emplace_back(Texture::createTexture(img));
    }
    queueRecreateUboBuffer = true;
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
