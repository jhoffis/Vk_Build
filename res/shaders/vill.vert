#version 450

layout(binding = 0) uniform UniformBufferObject {
    vec3 pos;
    float aspect;
    int selected;
//    mat4 model;
//    mat4 view;
//    mat4 proj;
} ubo[100];

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec2 inPos;
layout(location = 4) in uint inIndex;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out int fragTextureIndex;

vec3 pos(int n)
{
    int index = n;
	for (int i = 0; i < ubo.length(); i++) {
		if (i == index) return ubo[index].pos;
	}
    return ubo[index].pos;
}


void main() {
    gl_Position = vec4(inPosition.x, inPosition.yz, 1.0);// ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);

    // fragColor.r = 0.5*ubo.pos.y + .1 + ubo.pos.z;
    //  gl_Position.z = fragColor.r;
    int num = gl_InstanceIndex;
// fragColor.rgb = a(num).selected == 1 ? vec3(1.0) : vec3(0.0);
    // fragColor.r = 1.0;
    gl_Position.x += pos(num).x;
    gl_Position.y += -pos(num).y;
    fragColor.r = 1. ;
    fragColor.b = 0.0;
    fragColor.g = 0.0; // num / 10.0 + 0.1;
    gl_Position.x /= ubo[0].aspect;
    gl_Position.x *= 2.;
    gl_Position.y *= 2.;
    gl_Position.x -= 1.;
    gl_Position.y += 1.;
    // gl_Position.z = 0.5*ubo.pos.y + .1 + ubo.pos.z;
    fragTexCoord = inTexCoord;

    fragTextureIndex = 1;
    if (num == 1) {
        fragTextureIndex = 0;
    }
}
