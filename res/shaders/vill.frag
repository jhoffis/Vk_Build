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
        outColor = mix(texture(texSampler[1], fragTexCoord), vec4(fragColor.rgb, 1.), .1);
    }
}

