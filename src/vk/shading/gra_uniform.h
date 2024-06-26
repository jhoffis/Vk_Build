#pragma once

#include <cstdint>
#include "window.h"
#include "rendering/Entity.h"
#include "vk/gra_setup.h"
#include "models/consts/ShaderName.h"
#include "gra_vertex.h"
#include <functional>
#include <vector>

namespace Gra_Uniform {

    struct UBOMem {

        uint32_t count{}; // number of allocations within one large offset
        // uint32_t setOffset{}; // offset between each actual ubo if there are more than 1 in count.

        uint32_t amount{}; // number of allocations after each other. Division of the buffer.
        uint32_t offset{}; // distance between each allocated n mem. Not less than minUniformBufferOffsetAlignment.
        uint32_t range{};  // bytes actually allocated (sizeof)
        void *uboStruct{}; // the data the buffer is filled with.
        std::vector<VkBuffer> uniformBuffers{}; // Size = MAX_FRAMES_IN_FLIGHT
        std::vector<VkDeviceMemory> uniformBuffersMemory{}; // Actual memory location. Size = MAX_FRAMES_IN_FLIGHT

        void destroy() const {
            for (size_t i = 0; i < uniformBuffers.size(); i++) {
                vkDestroyBuffer(Gra::m_device, uniformBuffers[i], nullptr);
                vkFreeMemory(Gra::m_device, uniformBuffersMemory[i], nullptr);
            }
        }
    };

    struct UBOParams {
        const int sizeofUBOStruct;
        const std::function<void(Gra_Uniform::UBOMem*, const std::shared_ptr<Entity> &)> update;
    };

    UBOMem createUniformBuffers(uint32_t amount, uint32_t count, uint32_t sizeOfUBO);
    void updateUniformBuffer(const UBOMem &uboMem,
                             uint32_t currentSwapImage,
                             uint32_t entityIndex);
    void clearRestUniformBuffer(const UBOMem &uboMem,
                                uint32_t currentSwapImage,
                                uint32_t startEntityIndex);
}
