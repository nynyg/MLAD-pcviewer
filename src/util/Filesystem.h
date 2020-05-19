#pragma once

#include "iostream"
#include "vector"

namespace filesystem {

    bool cmp(const std::string &a, const std::string &b);

    void getDirContent(const std::string &dirPath,
                       std::vector<std::string> &dirContent);

} // namespace filesystem
