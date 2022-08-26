#pragma once

#include <vector>
#include "src/window.h"

namespace Gra {
    VkShaderModule createShaderModule(const std::vector<char>& code);
}