#pragma once

#include <vector>
#include <string>

std::vector<char> readFile(const std::string &filename);
void watchDir(const std::string path);