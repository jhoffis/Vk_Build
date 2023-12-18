#include "file_util.h"
#include "vk/pipeline/gra_pipeline.h"
#include <stdexcept>
#include <fstream>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <filesystem>

HANDLE m_resThread;

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

void watchDir() {
#ifdef WIN32
    m_resThread = CreateThread(nullptr,
                0,
                [](LPVOID lpParam) -> DWORD {
                    HANDLE hDirChange = FindFirstChangeNotificationA(
                            "res/shaders",
                            false,
                            FILE_NOTIFY_CHANGE_LAST_WRITE
                    );
                    if (hDirChange == INVALID_HANDLE_VALUE) {
                        throw std::runtime_error("failed to watch directory: ");
                    }
                    do {
                        DWORD waitResult = WaitForSingleObject(hDirChange, INFINITE);
                        if (waitResult == WAIT_OBJECT_0) {
                            std::cout << "Noticed change" << std::endl;

                            Raster::compilePipelines(true);

                            // Wait so you don't redo the same message and
                            // reissue the FindFirstChangeNotificationA to continue monitoring
                            std::this_thread::sleep_for(std::chrono::milliseconds(500));
                            FindNextChangeNotification(hDirChange);
                        } else {
                            std::cerr << "WaitForSingleObject failed." << std::endl;
                            break;
                        }
                    } while (true);
                    FindCloseChangeNotification(hDirChange);
                    return 0;
                },
                nullptr,
                0,
                nullptr);
#endif
}

void unwatchDir() {
    CloseHandle(m_resThread);
}

void makeSureDirExists(const char *folder) {
    if (!std::filesystem::exists(folder)) {
        std::filesystem::create_directory(folder);
    }
}
