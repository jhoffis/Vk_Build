#pragma once

#include <vector>
#include <string>
#include <memory>

std::string getFilename(const std::string &path);

std::vector<char> readFile(const std::string &filename);
void writeFile(const std::string& filename, const std::string& content);

void makeSureDirExists(const char *folder);

std::string getEnvVar(const std::string &varName);

void runExecutable(std::string command);