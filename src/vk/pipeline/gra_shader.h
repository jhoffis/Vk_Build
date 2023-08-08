#pragma once

#include <vector>
#include "src/window.h"

namespace Shader {
    VkShaderModule createShaderModule(const std::vector<char> &code);
}