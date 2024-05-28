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
};

std::vector<const FolderParams *> folderParamsThreads{};

#include <thread>

#ifdef _WIN32
std::vector<HANDLE> folderWatcherThreads{};
#elif __linux__

#include <memory>

std::vector<std::shared_ptr<std::thread>> folderWatcherThreads{};
#endif

void createFolderWatcher(const FolderParams *params) {
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
    folderParamsThreads.emplace_back(params);
#elif __linux__

    folderWatcherThreads.emplace_back(
            std::make_shared<std::thread>(
                    std::thread([params]() {
                        for(;;) {
                            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                            params->folderChange();
                        }
                    })
            )
    );
#endif
}


void watchDir() {
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
}

void unwatchDir() {
#ifdef _WIN32
    for (auto threadHandle: folderWatcherThreads) {
        CloseHandle(threadHandle);
    }
#elif __linux__
    for (auto threadHandle: folderWatcherThreads) {
        threadHandle->join();
    }
#endif
    for (auto params: folderParamsThreads) {
        delete params;
    }
}

#endif
