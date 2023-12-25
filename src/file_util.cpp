#include "file_util.h"
#include "vk/pipeline/gra_pipeline.h"
#include "math/math_stuff.h"
#include "timer_util.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <thread>
#include <filesystem>
#include <functional>

struct FolderParams {
    const char *folder;
    const std::function<void()> folderChange;
};

std::vector<HANDLE> folderWatcherThreads{};
std::vector<const FolderParams *> folderParamsThreads{};


std::string getFilename(const std::string &path) {
    auto slashIndex = path.find_last_of('/');
    auto backslashIndex = path.find_last_of('\\');
    return path.substr(Math::maximus(slashIndex, backslashIndex) + 1);
}

std::vector<char> readFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file: " + filename);
    }

    auto fileSize = static_cast<std::streamsize>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

HANDLE createFolderWatcher(const FolderParams *params) {
    auto thread = CreateThread(nullptr,
                               0,
                               [](LPVOID lpParam) -> DWORD {
                                   auto folderParams = (FolderParams *) lpParam;
                                   HANDLE hDirChange = FindFirstChangeNotificationA(
                                           folderParams->folder,
                                           false,
                                           FILE_NOTIFY_CHANGE_LAST_WRITE
                                   );
                                   if (hDirChange == INVALID_HANDLE_VALUE) {
                                       throw std::runtime_error(std::format("failed to watch directory: {}", folderParams->folder));
                                   }
                                   do {
                                       DWORD waitResult = WaitForSingleObject(hDirChange, INFINITE);
                                       if (waitResult == WAIT_OBJECT_0) {
                                           std::cout << "Noticed change" << std::endl;

                                           folderParams->folderChange();

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
                               (LPVOID) params,
                               0,
                               nullptr);
    folderWatcherThreads.emplace_back(thread);
    folderParamsThreads.emplace_back(params);
    return thread;
}

void watchDir() {
#ifdef WIN32
    createFolderWatcher(new FolderParams{
            "../res/shaders",
            []() {
                auto timeCompare = Timer::nowMillisFile() - 1000;
                std::string path = "../res/shaders";
                for (const auto &entry: std::filesystem::directory_iterator(path)) {
                    auto entryPath = entry.path().string();
                    if (!(entryPath.ends_with(".vert") || entryPath.ends_with(".frag"))) {
                        continue;
                    }
                    auto lastWrite = Timer::toMillis(entry.last_write_time());
                    if (lastWrite < timeCompare) {
                        continue;
                    }

                    auto newPath = "res/shaders/" + getFilename(entryPath);
                    if (std::filesystem::exists(newPath)) {
                        std::filesystem::remove(newPath);
                    }
                    std::filesystem::copy(entryPath, newPath);
                }
                Raster::compilePipelines(true);
            }
    });
#endif
}

void unwatchDir() {
    for (auto threadHandle: folderWatcherThreads) {
        CloseHandle(threadHandle);
    }
    for (auto params: folderParamsThreads) {
        delete params;
    }
}

void makeSureDirExists(const char *folder) {
    if (!std::filesystem::exists(folder)) {
        std::filesystem::create_directory(folder);
    }
}

std::string getEnvVar(const std::string &varName) {
    char* buf = nullptr;
    size_t sz = 0;
    if (_dupenv_s(&buf, &sz, varName.c_str()) != 0 || buf == nullptr) {
        throw std::runtime_error("Could not find env " + varName);
    }
    std::string ret{buf};
    free(buf);
    return ret;
}

void runExecutable(std::string command) {

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Start the child process.
    if (!CreateProcess(nullptr,   // No module name (use command line)
                       const_cast<LPSTR>(command.c_str()),        // Command line
                       nullptr,           // Process handle not inheritable
                       nullptr,           // Thread handle not inheritable
                       FALSE,          // Set handle inheritance to FALSE
                       0,              // No creation flags
                       nullptr,           // Use parent's environment block
                       nullptr,           // Use parent's starting directory
                       &si,            // Pointer to STARTUPINFO structure
                       &pi)           // Pointer to PROCESS_INFORMATION structure
            ) {
        throw std::runtime_error(std::format("failed to run exe: {}", command));
    }

    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
