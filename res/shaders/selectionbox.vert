#version 450

layout(binding = 0) uniform UniformBufferObject {
    vec3 pos;
    float aspect;
//    mat4 model;
//    mat4 view;
//    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

vec2 positions[4] = vec2[](
vec2(-0.5, -0.5),
vec2(0.5, -0.5),
vec2(0.5, 0.5),
vec2(-0.5, 0.5)
);
//vec3 positions[4] = vec2[](
//vec3(-0.5, -0.5),
//vec3(0.5, -0.5),
//vec3(0.5, 0.5),
//vec3(-0.5, 0.5)
//);

void main() {
//    gl_Position = vec4(inPosition.xyz, 1.0);// ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
//    gl_Position.x += ubo.pos.x;
//    gl_Position.y += -ubo.pos.y;
//    gl_Position.x /= ubo.aspect;
//    gl_Position.x *= 2.;
//    gl_Position.y *= 2.;
//    gl_Position.x -= 1.;
//    gl_Position.y += 1.;
    //zoom:
//    gl_Position.x *= ubo.pos.z;
//    gl_Position.y *= ubo.pos.z;

    if (gl_VertexIndex < 2) {
        gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    } else {
        gl_Position = vec4(inPosition.xyz, 1.0);
    }

    fragColor = inColor; // vec3(1,0,0);
    fragTexCoord = inTexCoord;
}