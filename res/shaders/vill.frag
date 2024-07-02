#version 450

layout(binding = 1) uniform sampler2D texSampler[2];

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in flat int fragTextureIndex;

layout(location = 0) out vec4 outColor;

void main() {
//    outColor = texture(texSampler, fragTexCoord);
    outColor = vec4(fragColor, 1.);
    if (fragTextureIndex == 0) {
        outColor = mix(texture(texSampler[0], fragTexCoord), vec4(fragColor.rgb, 1.), .1);
    } else if (fragTextureIndex == 1) {
        outColor = mix(texture(texSampler[0], fragTexCoord), vec4(fragColor.rgb, 1.), .1);
    }
}
/*
validation layer: Validation Error: [ VUID-VkPipelineLayoutCreateInfo-graphicsPipelineLibrary-06753 ] | MessageID = 0x57ab6143 | vkCreatePipelineLayout(): pCreateInfo->pSetLayouts[0] is VK_NULL_HANDLE, but VK_EXT_graphics_pipeline_library is not enabled. The Vulkan spec states: If graphicsPipelineLibrary is not enabled, elements of pSetLayouts must be valid VkDescriptorSetLayout objects (https://vulkan.lunarg.com/doc/view/1.3.283.0/linux/1.3-extensions/vkspec.html#VUID-VkPipelineLayoutCreateInfo-graphicsPipelineLibrary-06753)
*/
