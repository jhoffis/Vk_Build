//
// Created by Jens Benz on 18.09.2022.
//


#include <vector>
#include "gra_vertex.h"
#include "src/vk/gra_setup.h"
#include "gra_memory_utils.h"

namespace Gra {

    // std::vector<Vertex> Gvertices;
    // std::vector<uint32_t> Gindices;

    // VkBuffer m_vertexBuffer;
    // VkBuffer m_indexBuffer;
    struct VertexBuff {
        VkBuffer buffer;
        VkDeviceMemory deviceMemory;
    };
    std::vector<VertexBuff> m_bufferMemory;

    VkBuffer createVertexBuffer(std::vector<Vertex> vertices) {
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                     VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                     stagingBufferMemory);

        void *data;
        vkMapMemory(m_device, stagingBufferMemory, 0, bufferSize, 0,
                    &data);
        memcpy(data, vertices.data(), (size_t) bufferSize);
        vkUnmapMemory(m_device, stagingBufferMemory);

        VkBuffer vertexBuffer{};
        VkDeviceMemory vertexBufferMemory{};
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                 VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer,
                     vertexBufferMemory);
        m_bufferMemory.push_back(VertexBuff{
            .buffer = vertexBuffer,
            .deviceMemory = vertexBufferMemory
        });

        copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

        vkDestroyBuffer(m_device, stagingBuffer, nullptr);
        vkFreeMemory(m_device, stagingBufferMemory, nullptr);

        /*
           It should be noted that in a real world application, you’re not supposed
           to actually call vkAllocateMemory for every individual buffer. The
           maximum number of simultaneous memory allocations is limited by the
           maxMemoryAllocationCount physical device limit, which may be as low as
           4096 even on high end hardware like an NVIDIA GTX 1080.

           The right way to allocate memory for a large number of objects at the same time is to
           create a custom allocator that splits up a single allocation among many different
           objects by using the offset parameters that we’ve seen in many functions.
           You can either implement such an allocator yourself, or use the VulkanMem-
           oryAllocator library provided by the GPUOpen initiative.

           You should allocate multiple resources like buffers from a single memory allocation,
           but in fact you should go a step further. Driver developers recommend that you also
           store multiple buffers, like the vertex and index buffer, into a single VkBuffer
           and use offsets in commands like vkCmdBindVertexBuffers.
           The advantage is that your data is more cache friendly in that case,
           because it's closer together.
           It is even possible to reuse the same chunk of memory for multiple resources if they
           are not used during the same render operations, provided that their data is refreshed,
           of course.
           This is known as aliasing and some Vulkan functions have explicit flags to specify
           that you want to do this.
         */

        return vertexBuffer;
    }

    VkBuffer createIndexBuffer(std::vector<uint32_t> indices) {
        // The bufferSize is now equal to the
        // number of indices times the size of the index type, either uint16_t or uint32_t.
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                     VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                     stagingBufferMemory);

        void *data;
        vkMapMemory(m_device, stagingBufferMemory, 0, bufferSize, 0,
                    &data);
        memcpy(data, indices.data(), (size_t) bufferSize);
        vkUnmapMemory(m_device, stagingBufferMemory);

        VkBuffer indexBuffer{};
        VkDeviceMemory indexBufferMemory{};
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                 VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer,
                     indexBufferMemory);
        m_bufferMemory.push_back(VertexBuff{
            .buffer = indexBuffer,
            .deviceMemory = indexBufferMemory
        });

        copyBuffer(stagingBuffer, indexBuffer, bufferSize);

        vkDestroyBuffer(m_device, stagingBuffer, nullptr);
        vkFreeMemory(m_device, stagingBufferMemory, nullptr);
        return indexBuffer;
    }


    void cleanupVertex() {
        for (auto & mem : m_bufferMemory) {
            vkDestroyBuffer(m_device, mem.buffer, nullptr);
            vkFreeMemory(m_device, mem.deviceMemory, nullptr);
        }
    }
}


