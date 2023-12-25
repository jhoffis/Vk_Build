#pragma once

#include <vector>
#include <string>
#include <memory>

std::string getFilename(const std::string &path);

std::vector<char> readFile(const std::string &filename);
void watchDir();
void unwatchDir();

void makeSureDirExists(const char *folder);

std::string getEnvVar(const std::string &varName);

void runExecutable(std::string command);