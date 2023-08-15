//
// Created by jonhof on 8/14/2023.
//

#include <iostream>
#include "Mesh.h"
#include "vk/shading/gra_memory_utils.h"

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
namespace Mesh {
    std::vector<VkDeviceMemory> bufferMemToClean{};

    void createVertexBuffer(Mesh *mesh) {
        VkDeviceSize bufferSize = sizeof(mesh->vertices[0]) * mesh->vertices.size();
        std::cout << mesh->vertices.size() << " vertices" << std::endl;
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        Gra::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                          stagingBufferMemory);

        void *data;
        vkMapMemory(Gra::m_device, stagingBufferMemory, 0, bufferSize, 0,
                    &data);
        memcpy(data, mesh->vertices.data(), (size_t) bufferSize);
        vkUnmapMemory(Gra::m_device, stagingBufferMemory);

        VkDeviceMemory mem{};
        VkBuffer vertexBuffer{};
        Gra::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, // Device local = at the gpu
                          mem);
        bufferMemToClean.emplace_back(mem);

        Gra::copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
        mesh->vertexBuffer = vertexBuffer;

        vkDestroyBuffer(Gra::m_device, stagingBuffer, nullptr);
        vkFreeMemory(Gra::m_device, stagingBufferMemory, nullptr);
    }

    void createIndexBuffer(Mesh *mesh) {
        // The bufferSize is now equal to the
        // number of indices times the size of the index type, either uint16_t or uint32_t.
        VkDeviceSize bufferSize = sizeof(mesh->indices[0]) * mesh->indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        Gra::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                          stagingBufferMemory);

        void *data;
        vkMapMemory(Gra::m_device, stagingBufferMemory, 0, bufferSize, 0,
                    &data);
        memcpy(data, mesh->indices.data(), (size_t) bufferSize);
        vkUnmapMemory(Gra::m_device, stagingBufferMemory);

        VkDeviceMemory mem{};
        VkBuffer indexBuffer{};
        Gra::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                      VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer,
                          mem);
        bufferMemToClean.emplace_back(mem);

        Gra::copyBuffer(stagingBuffer, indexBuffer, bufferSize);
        mesh->indexBuffer = indexBuffer;

        vkDestroyBuffer(Gra::m_device, stagingBuffer, nullptr);
        vkFreeMemory(Gra::m_device, stagingBufferMemory, nullptr);
    }

    void init(Mesh *mesh) {
        createVertexBuffer(mesh);
        createIndexBuffer(mesh);
    }

    void cleanup() {
        for (auto mem: bufferMemToClean) {
            vkFreeMemory(Gra::m_device, mem, nullptr);
        }
    }
}