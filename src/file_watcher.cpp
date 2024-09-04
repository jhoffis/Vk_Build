#include "file_watcher.h"

#ifdef RMDEV

#include <functional>
#include <filesystem>
#include "file_util.h"
#include "timer_util.h"
#include "vk/pipeline/gra_pipeline.h"

struct FolderParams {
    const char *folder;
    const std::function<void()> folderChange;
    bool end = false;
};

std::vector<FolderParams *> folderParamsThreads{};

#include <thread>
#include <iostream>

#ifdef _WIN32
std::vector<HANDLE> folderWatcherThreads{};
#elif __linux__
std::vector<std::shared_ptr<std::thread>> folderWatcherThreads{};
#endif

void createFolderWatcher(FolderParams *params) {
#ifdef _WIN32
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
#elif __linux__

    folderWatcherThreads.emplace_back(
            std::make_shared<std::thread>(
                    std::thread([params]() {
                        for(;;) {
                            if (params->end) break;
                            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                            params->folderChange();
                        }
                        delete params;
                    })
            )
    );
#endif
    folderParamsThreads.emplace_back(params);
}


void watchDir() {
#if _MSC_VER
                const static std::string path = "../../../res/shaders";
#else
                const static std::string path = "../res/shaders";
#endif
    createFolderWatcher(new FolderParams{
            path.c_str(),
            []() {
                auto timeCompare = Timer::nowMillisFile() - 5000;
                bool found = false;
                for (const auto &entry: std::filesystem::directory_iterator(path)) {
                    auto entryPath = entry.path().string();
                    if (!(entryPath.ends_with(".vert") || entryPath.ends_with(".frag"))) {
                        continue;
                    }
                    auto lastWrite = Timer::toMillis(entry.last_write_time());
                    if (lastWrite < timeCompare) {
                        continue;
                    }
                    found = true;
                    auto newPath = "res/shaders/" + getFilename(entryPath);
                    if (std::filesystem::exists(newPath)) {
                        std::filesystem::remove(newPath);
                    }
                    std::filesystem::copy(entryPath, newPath);
                }
                if (found)
                    Raster::compilePipelines(true);
            }
    });
}

void unwatchDir() {
#ifdef _WIN32
    for (auto threadHandle: folderWatcherThreads) {
        CloseHandle(threadHandle);
    }
    for (auto params: folderParamsThreads) {
        delete params;
    }
#elif __linux__
    for (auto params: folderParamsThreads) {
        params->end = true;
    }
    for (auto threadHandle: folderWatcherThreads) {
        threadHandle->join();
    }
#endif
}

#endif
