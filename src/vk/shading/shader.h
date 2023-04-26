//
// Created by jonhof on 4/26/2023.
//

#ifndef VULKAN_SHADER_H
#define VULKAN_SHADER_H


#include "gra_uniform.h"

class Shader {

private:
    Gra::Uniform ubo{};

    // Det er vel kanskje greit å deklarere en ny struct for hver shader? Kanskje... hm. nja hva om jeg ønsker å ha shader editing hotreloading?

    Shader(Texture::TexData tex) {
        createDescriptorSetLayout(ubo);
        createDescriptorSets(tex, ubo);
    }
};


#endif //VULKAN_SHADER_H
