#include "paths.h"
#include <filesystem>

namespace Paths {
    
    std::string load(const char *name, const char *dir) {
        std::string currPath = std::filesystem::current_path().string().append(dir);
        // const char *realPath = reinterpret_cast<const char *>(currPath.append(name).c_str());
        return currPath.append(name);
    }

    std::string pics(const char *name) {
        return load(name, "/res/pics/");
    }

    std::string models(const char *name) {
        return load(name, "/res/models/");
    }

} // Gra