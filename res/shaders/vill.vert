#version 450

layout(binding = 0) uniform UniformBufferObject {
    vec3 pos;
    float aspect;
    int selected;
//    mat4 model;
//    mat4 view;
//    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec2 inPos;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    gl_Position = vec4(inPosition.x, inPosition.yz, 1.0);// ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    gl_Position.x += ubo.pos.x;
    gl_Position.y += -ubo.pos.y + 0.1*inPos.y;
    gl_Position.x /= ubo.aspect;
    gl_Position.x *= 2.;
    gl_Position.y *= 2.;
    gl_Position.x -= 1.;
    gl_Position.y += 1.;

   // fragColor.r = 0.5*ubo.pos.y + .1 + ubo.pos.z;
 //  gl_Position.z = fragColor.r;

    fragColor.rgb = ubo.selected == 1 ? vec3(1.0) : vec3(0.0);
if (gl_InstanceIndex == 0) {
    fragColor.b = 1.0;
}
if (gl_InstanceIndex == 1) {
    fragColor.r = 1.0;
}
    gl_Position.z = 0.5*ubo.pos.y + .1 + ubo.pos.z;
    fragTexCoord = inTexCoord;
}
