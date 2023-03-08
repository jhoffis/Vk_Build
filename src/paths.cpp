#include "paths.h"
#include <filesystem>

namespace Paths {
    
    std::string load(const char *dir, const char *name) {
        return std::filesystem::current_path().string().append(dir).append(name);
    }

    std::string pics(const char *name) {
        return load("/res/pics/", name);
    }

    std::string models(const char *name) {
        return load("/res/models/", name);
    }

} // Paths