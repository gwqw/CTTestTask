#pragma once

#include <vector>
#include <string>

inline char getSlash() {
#ifndef __linux__
    return '\\';
#else
    return '/';
#endif
}

bool isCorrectFileName(const std::string& filename, int start_ep, int end_ep);
std::vector<std::string> getFileList(const std::string& input_path, int start_ep, int end_ep);

