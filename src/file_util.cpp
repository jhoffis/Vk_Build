#include "file_util.h"
#include <stdexcept>
#include <fstream>
#include <Windows.h>
#include <iostream>

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
                     auto hDirChange = FindFirstChangeNotificationA(
                             "res/shaders",
                             true,
                             FILE_NOTIFY_CHANGE_LAST_WRITE
                     );
                     if (hDirChange == INVALID_HANDLE_VALUE) {
                         throw std::runtime_error("failed to watch directory: ");
                     }

                     do {
                         auto waitResult = WaitForSingleObject(hDirChange, INFINITE);

                         if (waitResult == WAIT_OBJECT_0) {
                            std::cout << "Noticed change" << std::endl;
                             // Reissue the FindFirstChangeNotificationA to continue monitoring
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
