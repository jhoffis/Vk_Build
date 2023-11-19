#include "file_util.h"
#include <thread>
#include <stdexcept>
#include <fstream>
#include <Windows.h>

std::vector<char> readFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file: " + filename);
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

void watchDir(const std::string path) {
#ifdef WIN32
    std::thread watcherThread([](auto path) {
        HANDLE hDirChange = FindFirstChangeNotificationA(
                path.c_str(),
                true,
                FILE_NOTIFY_CHANGE_LAST_WRITE
        );
        if (hDirChange == INVALID_HANDLE_VALUE) {
            throw std::runtime_error("failed to watch directory: " + path);
        }

        WaitForSingleObject(hDirChange, INFINITE);
    }, path);
#endif
}
