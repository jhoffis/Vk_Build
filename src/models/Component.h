#pragma once

#include <string>
#include <vulkan/vulkan_core.h>
#include <vector>
#include "math/Vec3.h"
#include "math/Vec2.h"
#include "vk/shading/gra_vertex.h"
#include "rendering/Model.h"
#include "SelectionBoxModel.h"

//struct Component {
//    virtual void setup() = 0;
//    virtual void bind() = 0;
//    virtual void initFillDescriptor() = 0;
//    virtual void fillDescriptor() = 0;
//};
//
//struct ImageComp : Component {
//    std::string textureName;
//    float w{};
//    float h{};
//    VkImageView texImageView;
//
//    void setup();
//    void bind();
//    void initFillDescriptor();
//    void fillDescriptor();
//};
//
//struct UboComp : Component {
//    std::string textureName;
//
//    void setup();
//    void bind();
//    void initFillDescriptor();
//    void fillDescriptor();
//};
//
//struct ShaderInfo {
//    std::string name{};
//    std::vector<std::string> textures{};
//    std::vector<Vec2> inVec2s{};
//    std::vector<Vec3> inVec3s{};
//};
//
