#include "Model.h"
#include <iostream>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vulkan/vulkan_core.h>

#include "camera.h"
#include "models/Map.h"
#include "models/SelectionBoxModel.h"
#include "timer_util.h"
#include "vk/drawing/gra_drawing.h"
#include "vk/gra_descriptors.h"
#include "vk/gra_setup.h"
#include "vk/presentation/gra_swap_chain.h"
#include "vk/shading/gra_uniform.h"
#include "vk/shading/gra_vertex.h"

void grassUpdateRenderUbo(Gra_Uniform::UBOMem *uboMem,
                          const std::shared_ptr<Entity> &entity,
                          const uint16_t index) {
    static_cast<Gra::UniformBufferObject*>(uboMem->uboStruct)[index] = {
        .pos = entity->pos - Camera::m_cam.pos,
        .aspect = Gra::m_swapChainAspectRatio,
        .selected = entity->selected,
    };
}

std::vector<Model *> m_renderModels{};
Model Shaders::m_grassModel("grass");
Model Shaders::m_houseModel("house");
/* grassUpdateRenderUbo,
                            {
                                vert_ubo,
                                frag_image,
                            },
                            sizeof(Gra::UniformBufferObject), 0, 0,
                            {"grass.png"});
Model Shaders::m_houseModel("grass", grassUpdateRenderUbo,
                            {
                                vert_ubo,
                                frag_image,
                            },
                            sizeof(Gra::UniformBufferObject), 0, 0,
                            {"house.png"});
*/
Model Shaders::m_villModel("vill"); 
Model Shaders::m_selectionBoxModel("selectionbox");
/*"vill",
                           grassUpdateRenderUbo,
                           {
                               vert_ubo,
                               frag_image,
                           },
                           sizeof(Gra::UniformBufferObject),
                           0,
                           0,
                           {"unit.png"}};*/

// Model Shaders::m_selectionBoxModel{}; 
/*
    "selectionbox",
    [](auto uboMem, auto entity) {
      SelectionBox::m_ubo.aspect = Gra::m_swapChainAspectRatio;
      SelectionBox::m_ubo.resolution.x =
          Window::WIDTH; // kanskje monitor size istedet?
      SelectionBox::m_ubo.resolution.y = Window::HEIGHT;
      SelectionBox::m_ubo.posCam.x = Camera::m_cam.pos.x;
      SelectionBox::m_ubo.posCam.y = Camera::m_cam.pos.y;
      uboMem->uboStruct = &SelectionBox::m_ubo;
    },
    {
        vert_ubo,
    },
    sizeof(SelectionBox::SelectionBoxUBO),
    128,
    128,
    {}};*/
/*
void replaceTextures(const std::vector<std::string> &textures,
                     std::vector<VkImageView> &texImageViews) {
  texImageViews.clear();
  auto newTexs = Texture::getTexImageViews(textures);
  for (const auto &tex : newTexs) {
    texImageViews.emplace_back(tex);
  }
}
*/
/*
void updateDescriptorSet(int index,
                         const std::vector<VkImageView> &texImageViews,
                         const Gra_Uniform::UBOMem &uboMem,
                         const std::vector<ShaderComponentOrder> &order,
                         const std::vector<VkDescriptorSet> &descriptorSets) {
  auto n = 2 * index + Drawing::currSwapFrame;
  // setup phase
  std::vector<VkDescriptorImageInfo> imageInfos{texImageViews.size()};
  for (int i = 0; i < imageInfos.size(); i++) {
    imageInfos[i] = VkDescriptorImageInfo{
        .sampler = Texture::m_textureSampler,
        .imageView = texImageViews[i],
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    };
  }

  VkDescriptorBufferInfo bufferInfo{};
  bufferInfo.buffer =
      uboMem.uniformBuffers[n % Gra::MAX_FRAMES_IN_FLIGHT]; // TODO Her er
                                                            // bindingen til
                                                            // ubo o.l.
  bufferInfo.offset =
      uboMem.offset *
      static_cast<int>(std::floor(
          static_cast<float>(n) / // Here it picks out the mem it refers to!
          static_cast<float>(Gra::MAX_FRAMES_IN_FLIGHT)));
  bufferInfo.range = uboMem.range;

  std::vector<VkWriteDescriptorSet> descriptorWrites{
      static_cast<size_t>(order.size())};

  int nImg = 0; // gjør dette for hver entity basically.
  for (auto a = 0; a < order.size(); a++) {
    if (order[a] == vert_ubo) {
      descriptorWrites[a].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      descriptorWrites[a].dstSet = descriptorSets[n];
      descriptorWrites[a].dstBinding = a;
      descriptorWrites[a].dstArrayElement = 0;
      descriptorWrites[a].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      descriptorWrites[a].descriptorCount = 1;
      descriptorWrites[a].pBufferInfo = &bufferInfo;
    } else if (order[a] == frag_image) {
      descriptorWrites[a].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      descriptorWrites[a].dstSet = descriptorSets[n];
      descriptorWrites[a].dstBinding = a;
      descriptorWrites[a].dstArrayElement = 0;
      descriptorWrites[a].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      descriptorWrites[a].descriptorCount = 1;
      descriptorWrites[a].pImageInfo = &imageInfos[nImg];
      nImg++;
    }
  }

  // IT SAYS UPDATE. Can I run update on a singular desciptor without having
  // to delete and recreate everything?
  vkUpdateDescriptorSets(Gra::m_device,
                         static_cast<uint32_t>(descriptorWrites.size()),
                         descriptorWrites.data(), 0, nullptr);
}
*/

/*
 * Extremely slow method here.
 */
/*
std::vector<VkDescriptorSet> Model::createDescriptorSets() const {
  auto size = Gra::MAX_FRAMES_IN_FLIGHT *
              uboMem.amount; // Dette er amount: auto amount =
                             // MyMath::nextPowerOfTwo(entitiesSize); Men...
                             // why? Hvorfor ikke bare ha mengden entities?
                             // Fordi da må man kjøre denne for hver nye entity
                             // som lages, men man sparer mange fps av det da...
  std::vector<VkDescriptorSetLayout> layouts(size, descriptorSetLayout);
  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = pool;
  allocInfo.descriptorSetCount =
      static_cast<uint32_t>(size); // must be at least 1
  allocInfo.pSetLayouts = layouts.data();

  std::vector<VkDescriptorSet> descriptorSets{};
  descriptorSets.resize(size);
  auto res = vkAllocateDescriptorSets(
      Gra::m_device, &allocInfo,
      descriptorSets.data()); // is this deleted ever? Yes, via pool
  if (res != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate descriptor sets!");
  }

  std::vector<VkImageView> texImageViews{};
  std::vector<VkImageView> texImageViews2{};
  replaceTextures({"house.png"}, texImageViews2);

  auto ogSwapFrame = Drawing::currSwapFrame;

  for (auto i = 0; i < uboMem.amount; i++) {
    // TODO here we can change the imageInfo to the one you want
    // because the var size is the amount of entities!!!
    if (order.size() == 2) {
      Drawing::currSwapFrame = 0;
      updateDescriptorSet(i, texImageViews2, uboMem, order, descriptorSets);
      Drawing::currSwapFrame = 1;
      updateDescriptorSet(i, texImageViews2, uboMem, order, descriptorSets);
    } else {
      Drawing::currSwapFrame = 0;
      updateDescriptorSet(i, texImageViews, uboMem, order, descriptorSets);
      Drawing::currSwapFrame = 1;
      updateDescriptorSet(i, texImageViews, uboMem, order, descriptorSets);
    }
  }

  Drawing::currSwapFrame = ogSwapFrame;
  return descriptorSets;
}
*/
Model::Model(const std::string &shaderName) : shaderName(std::move(shaderName)) {
  m_renderModels.emplace_back(this);
}

void Model::init(const uint16_t countInstances, const int w, const int h) {
    cmdBuffer.init();

    // auto w = 128; // FIXME har ingen sammenheng med textureSize
    // auto h = 128;
    // TODO maybe support multiple vertex buffers?
    mesh.init(w, h, countInstances);
    Gra::createVertexBuffer(&mesh);
    Gra::createInstanceBuffer(&mesh);
    Gra::createIndexBuffer(&mesh);

    createPipeline();
    updateRenderUbo = grassUpdateRenderUbo;
    initRenderUbo = [](auto uboMem) {
        delete static_cast<Gra::UniformBufferObject *>(uboMem->uboStruct);
        uboMem->uboStruct = new Gra::UniformBufferObject[uboMem->count];
    };
}

void Model::runRecreateUbo() {
    auto entitiesSize = static_cast<int>(entities.size());
    if (entitiesSize <= box.uboMem.amount * box.uboMem.count)
        return;

    // liste med alle referanser til ubos - bare utvid vector listen med descSets
    // og så bruk currswapframe for å tegne alle.
    auto amount = MyMath::nextPowerOfTwo(entitiesSize);
    if (amount == box.uboMem.amount)
        return;
    std::cout << "recreates ubo to size " << amount << std::endl;
    vkDeviceWaitIdle(Gra::m_device); // TODO maybe replace this
    box = Gra_desc::recreateDescriptorBox(box, amount);
}


void Model::sort() {
    // FIXME Windows Visual studio does not like this... 
    // std::sort(entities.begin(), entities.end(),
    //        [](auto a, auto b) { return a->pos.y > b->pos.y; });
}


std::shared_ptr<Entity> Model::spawn(Vec2 mapPos, std::string texture) {
    mapPos = Map::mapToWorldCoordinates(mapPos);

    auto entity = std::make_shared<Entity>(
            Entity{
            .pos = {mapPos.x, mapPos.y, 0},
            .mesh = &mesh,
            .sprite = {texture}
            });
    entities.emplace_back(entity); // TODO
    return entity;
}

void Model::spawn(const std::shared_ptr<Entity> &entity) {
    entities.emplace_back(entity);
}


VkCommandBuffer Model::renderMeshes(uint32_t imageIndex) {
    if (queueRecreateUboBuffer) {
        queueRecreateUboBuffer = false;
        // runRecreateUbo();
    }
    if (entities.size() == 0)
        return nullptr;
    runRecreateUbo();
    auto n = 0;
    for (auto i = 0; i < entities.size(); i += box.uboMem.count) {
        initRenderUbo(&box.uboMem);
        auto uboIndex = 0;
        for (int a = i; uboIndex < box.uboMem.count && a < entities.size(); a++) {
            auto entity = entities[a];
            if (!entity->visible)
                continue;
            updateRenderUbo(&box.uboMem, entity, uboIndex); // TODO maybe just return a ubostruct?
            uboIndex++;
        }

        Gra_Uniform::updateUniformBuffer(box.uboMem, Drawing::currSwapFrame, n); 
        n++;

        // TODO update other uniforms here like imgs
        // Vel, ved bare 1 entity så går fps fra 7000 til 2400.
        // TODO throw error if you're supposted to have sprites?? Maybe.
        // if (entities[i]->sprite.size() > 0 && !queueRecreateUboBuffer) {
        //     updateDescriptorSet(i, Texture::getTexImageViews(entities[i]->sprite),
        //             uboMem, order, descriptorSets);
        // }
    }
    Gra_Uniform::clearRestUniformBuffer(box.uboMem, Drawing::currSwapFrame, n);

    auto cmd = cmdBuffer.commandBuffers[Drawing::currSwapFrame];
    vkResetCommandBuffer(cmd, 0);
    Gra::recordCommandBuffer(cmd, imageIndex, mesh, pipeline, box.sets, entities.size(), box.uboMem);

    return cmd;
}

void Model::removeEntity(const std::shared_ptr<Entity> &sharedPtr) {
  entities.erase(std::remove(entities.begin(), entities.end(), sharedPtr),
                 entities.end());
  // recreateUboBuffer();
}

void Model::createPipeline() {
  pipeline = Raster::createGraphicsPipeline(box.layout, shaderName);
}

#ifdef RMDEV

std::vector<Raster::Pipeline> m_leftoverPipelines;

void recreateModelPipelines() {
  for (auto model : m_renderModels) {
    m_leftoverPipelines.emplace_back(model->pipeline);
    model->createPipeline();
  }
}

#endif

void Model::destroy() {
  vkDestroyCommandPool(Gra::m_device, cmdBuffer.commandPool, nullptr);
  Raster::destroyPipeline(pipeline);
  Gra_desc::destroyDescriptorBox(box);
}
void destroyModels() {
  for (auto model : m_renderModels) {
    model->destroy();
  }

#ifdef RMDEV
  for (auto pipeline : m_leftoverPipelines) {
    Raster::destroyPipeline(pipeline);
  }
#endif
}
