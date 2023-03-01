//
// Created by Jens Benz on 18.09.2022.
//


#include <vector>
#include "gra_vertex.h"
#include "src/vk/gra_setup.h"
#include "gra_memory_utils.h"

namespace Gra {

    std::vector<Vertex> Gvertices;
    std::vector<uint32_t> Gindices;

    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexBufferMemory;
    VkBuffer m_indexBuffer;
    VkDeviceMemory m_indexBufferMemory;

    void createVertexBuffer() {

        VkDeviceSize bufferSize = sizeof(Gvertices[0]) * Gvertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                     VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                     stagingBufferMemory);

        void *data;
        vkMapMemory(m_device, stagingBufferMemory, 0, bufferSize, 0,
                    &data);
        memcpy(data, Gvertices.data(), (size_t) bufferSize);
        vkUnmapMemory(m_device, stagingBufferMemory);

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                 VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertexBuffer,
                     m_vertexBufferMemory);

        copyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);

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
    }

    void createIndexBuffer() {
        // The bufferSize is now equal to the
        // number of indices times the size of the index type, either uint16_t or uint32_t.
        VkDeviceSize bufferSize = sizeof(Gindices[0]) * Gindices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                     VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                     stagingBufferMemory);

        void *data;
        vkMapMemory(m_device, stagingBufferMemory, 0, bufferSize, 0,
                    &data);
        memcpy(data, Gindices.data(), (size_t) bufferSize);
        vkUnmapMemory(m_device, stagingBufferMemory);

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                 VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_indexBuffer,
                     m_indexBufferMemory);

        copyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

        vkDestroyBuffer(m_device, stagingBuffer, nullptr);
        vkFreeMemory(m_device, stagingBufferMemory, nullptr);
    }


    void cleanupVertex() {
        vkDestroyBuffer(m_device, m_indexBuffer, nullptr);
        vkFreeMemory(m_device, m_indexBufferMemory, nullptr);

        vkDestroyBuffer(m_device, m_vertexBuffer, nullptr);
        vkFreeMemory(m_device, m_vertexBufferMemory, nullptr);
    }


}


