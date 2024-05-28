#include "file_util.h"
#include "math/math_stuff.h"
#include "timer_util.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <filesystem>


std::string getFilename(const std::string &path) {
    // TODO does not work on linux
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

void makeSureDirExists(const char *folder) {
    if (!std::filesystem::exists(folder)) {
        std::filesystem::create_directory(folder);
    }
}

std::string getEnvVar(const std::string &varName) {
    char* buf = nullptr;
    size_t sz = 0;
#ifdef _WIN32
    if (_dupenv_s(&buf, &sz, varName.c_str()) != 0 || buf == nullptr) {
        throw std::runtime_error("Could not find env " + varName);
    }
#endif
    std::string ret{buf};
    free(buf);
    return ret;
}

// TODO actually more like run command...
void runExecutable(std::string command) {
#ifdef _WIN32

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
#elif __linux__
    std::system(command.c_str());
#endif
}
