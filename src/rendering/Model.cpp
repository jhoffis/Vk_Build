#include "Model.h"
#include <iostream>
#include <stdexcept>
#include <thread>
#include <utility>

#include "camera.h"
#include "models/Map.h"
#include "models/SelectionBoxModel.h"
#include "timer_util.h"
#include "vk/drawing/gra_drawing.h"
#include "vk/gra_setup.h"
#include "vk/presentation/gra_swap_chain.h"
#include "vk/shading/gra_uniform.h"
#include "vk/shading/gra_vertex.h"

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
Model Shaders::m_grassModel("grass", grassUpdateRenderUbo,
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

Model Shaders::m_villModel{"vill",
                           grassUpdateRenderUbo,
                           {
                               vert_ubo,
                               frag_image,
                           },
                           sizeof(Gra::UniformBufferObject),
                           0,
                           0,
                           {"unit.png"}};

Model Shaders::m_selectionBoxModel{
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
    {}};

#ifdef RMDEV
std::vector<Raster::Pipeline> m_leftoverPipelines;
#endif

void replaceTextures(const std::vector<std::string> &textures,
                     std::vector<VkImageView> &texImageViews) {
  texImageViews.clear();
  for (const auto &tex : textures) {
    auto img = Texture::loadImage(tex.c_str());
    texImageViews.emplace_back(Texture::createTexture(img));
  }
}

void Model::setTextures(const std::vector<std::string> &textures) {
  replaceTextures(textures, texImageViews);
  queueRecreateUboBuffer = true;
}

void updateDescriptorSet(int i, const std::vector<VkImageView> &texImageViews,
                         const Gra_Uniform::UBOMem &uboMem,
                         const std::vector<ShaderComponentOrder> &order,
                         const std::vector<VkDescriptorSet> &descriptorSets) {

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
      uboMem.uniformBuffers[i % Gra::MAX_FRAMES_IN_FLIGHT]; // TODO Her er
                                                            // bindingen til
                                                            // ubo o.l.
  bufferInfo.offset =
      uboMem.offset *
      static_cast<int>(std::floor(
          static_cast<float>(i) / // Here it picks out the mem it refers to!
          static_cast<float>(Gra::MAX_FRAMES_IN_FLIGHT)));
  bufferInfo.range = uboMem.range;

  std::vector<VkWriteDescriptorSet> descriptorWrites{
      static_cast<size_t>(order.size())};

  int nImg = 0; // gjør dette for hver entity basically.
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
      descriptorWrites[a].descriptorType =
          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      descriptorWrites[a].descriptorCount = 1;
      descriptorWrites[a].pImageInfo = &imageInfos[nImg];
      nImg++;
    }
  }

  // IT SAYS UPDATE. Can I run update on a singular desciptor without having to
  // delete and recreate everything?
  vkUpdateDescriptorSets(Gra::m_device,
                         static_cast<uint32_t>(descriptorWrites.size()),
                         descriptorWrites.data(), 0, nullptr);
}

/*
 * Extremely slow method here.
 */
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

  for (auto i = 0; i < size;
       i++) { // TODO here we can change the imageInfo to the one you want
              // because the var size is the amount of entities!!!
    updateDescriptorSet(i, texImageViews, uboMem, order, descriptorSets);
  }

  std::vector<VkImageView> texImageViews2{};
  replaceTextures({"house.png"}, texImageViews2);
  updateDescriptorSet(0, texImageViews2, uboMem, order, descriptorSets);
  updateDescriptorSet(1, texImageViews2, uboMem, order, descriptorSets);
  return descriptorSets;
}

Model::Model(std::string shaderName,
             std::function<void(Gra_Uniform::UBOMem *,
                                const std::shared_ptr<Entity> &entity)>
                 updateRenderUbo,
             std::vector<ShaderComponentOrder> order, const int sizeOfUBO,
             const float overrideWidth, const float overrideHeight,
             std::vector<std::string> textures)
    : shaderName(std::move(shaderName)),
      updateRenderUbo(std::move(updateRenderUbo)), order(std::move(order)),
      sizeOfUBO(sizeOfUBO), overrideWidth(overrideWidth),
      overrideHeight(overrideHeight), textures(std::move(textures)) {
  m_renderModels.emplace_back(this);
}

void Model::init() {
  // TODO Alle disse er hardkodet til shaderen triangle mtp bindings og
  // attributes. Feks at de først har uniform buffer og så image sampler.
  cmdBuffer.init();

  pool = Gra_Uniform::createDescriptorPool(1);

  { // Shader stuff

    auto w = overrideWidth;
    auto h = overrideHeight;
    for (const auto &tex : textures) {
      auto img = Texture::loadImage(tex.c_str());
      texImageViews.emplace_back(Texture::createTexture(img));
      if (w == 0)
        w = static_cast<float>(img.w);
      if (h == 0)
        h = static_cast<float>(img.h);
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
    // descriptorSets = createDescriptorSets();
  }
  createPipeline();
}

std::shared_ptr<Entity> Model::spawn(Vec2 mapPos, std::string texture) {
  mapPos = Map::mapToWorldCoordinates(mapPos);

  auto entity = std::make_shared<Entity>(
      Entity{.pos = {mapPos.x, mapPos.y, 0}, .size = {width(), height()}});
  addEntity(entity);
  return entity;
}

void Model::runRecreateUbo() {
  // liste med alle referanser til ubos - bare utvid vector listen med descSets
  // og så bruk currswapframe for å tegne alle.
  auto entitiesSize = static_cast<int>(entities.size());
  auto amount = MyMath::nextPowerOfTwo(entitiesSize);
  if (amount == uboMem.amount)
    return;
  std::cout << "recreates ubo to size " << amount << std::endl;
  auto t0 = Timer::nowNanos();
  vkDeviceWaitIdle(Gra::m_device); // TODO maybe replace this
  auto t1 = Timer::nowNanos();
  vkDestroyDescriptorPool(Gra::m_device, pool, nullptr);
  auto t2 = Timer::nowNanos();
  pool = Gra_Uniform::createDescriptorPool(amount);
  auto t3 = Timer::nowNanos();
  uboMem.destroy();
  auto t4 = Timer::nowNanos();
  uboMem = Gra_Uniform::createUniformBuffers(amount, uboMem.range);
  auto t5 = Timer::nowNanos();
  descriptorSets = createDescriptorSets();
  auto t6 = Timer::nowNanos();
  Timer::printTimeDiffNanos(t0, t1);
  Timer::printTimeDiffNanos(t0, t2);
  Timer::printTimeDiffNanos(t0, t3);
  Timer::printTimeDiffNanos(t0, t4);
  Timer::printTimeDiffNanos(t0, t5);
  Timer::printTimeDiffNanos(t0, t6);
}

VkCommandBuffer Model::renderMeshes(uint32_t imageIndex) {
  if (queueRecreateUboBuffer) {
    queueRecreateUboBuffer = false;
    runRecreateUbo();
  }
  if (entities.size() == 0)
    return nullptr;

  auto n = 0;
  for (auto &entity : entities) {
    if (!entity->visible)
      continue;
    updateRenderUbo(&uboMem, entity); // TODO maybe just return a ubostruct?
    Gra_Uniform::updateUniformBuffer(uboMem, Drawing::currSwapFrame, n);
    // TODO update other uniforms here like imgs
    n++;
  }

  Gra_Uniform::clearRestUniformBuffer(uboMem, Drawing::currSwapFrame, n);

  auto cmd = cmdBuffer.commandBuffers[Drawing::currSwapFrame];
  vkResetCommandBuffer(cmd, 0);
  Gra::recordCommandBuffer(cmd, imageIndex, mesh, pipeline, descriptorSets);

  return cmd;
}

void Model::recreateUboBuffer() { queueRecreateUboBuffer = true; }

void Model::addEntity(const std::shared_ptr<Entity> &entity) {
  entities.emplace_back(entity);
  recreateUboBuffer();
}

void Model::addEntity(const std::shared_ptr<Entity> &entity, bool update) {
  entities.emplace_back(entity);
  if (update)
    recreateUboBuffer();
}

void Model::removeEntity(const std::shared_ptr<Entity> &sharedPtr) {
  entities.erase(std::remove(entities.begin(), entities.end(), sharedPtr),
                 entities.end());
  recreateUboBuffer();
}

void Model::spawn(float x, float y) {
  recreateUboBuffer();
  entities.emplace_back(std::make_shared<Entity>(
      Entity{.pos = {x, y, 0}, .size = {width(), height()}}));
}

void Model::createPipeline() {
  pipeline = Raster::createGraphicsPipeline(descriptorSetLayout, shaderName);
}

#ifdef RMDEV

void recreateModelPipelines() {
  for (auto model : m_renderModels) {
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
            [](auto a, auto b) { return a->pos.y > b->pos.y; });
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
