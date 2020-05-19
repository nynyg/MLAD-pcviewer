#include "Filesystem.h"
#include <algorithm>
#include <experimental/filesystem>
#include <ctime>

namespace filesystem {

    namespace stdfs = std::experimental::filesystem;

// compare function to be used for sorting, should return true if the first
// argument is less than the second
    bool cmp(const std::string &a, const std::string &b) {
        char c = '_';

        std::size_t pos = a.find(c);

        std::string first = a.substr(pos + 1);

        pos = b.find(c);

        std::string second = b.substr(pos + 1);

        if (first.size() > second.size())
            return false;
        else if (first.size() < second.size())
            return true;
        else {
            std::string::iterator firstItr = first.begin();
            std::string::iterator secondItr = second.begin();

            while (firstItr != first.end()) {
                if (*firstItr < *secondItr)
                    return true;
                else if (*firstItr > *secondItr)
                    return false;

                firstItr++;
                secondItr++;
            }
        }
        return true;
    }

    void getDirContent(const std::string &dirPath,
                       std::vector<std::string> &dirContent) {
        for (auto &p : stdfs::directory_iterator(dirPath)) {
            dirContent.emplace_back(p.path());
        }
        std::sort(dirContent.begin(), dirContent.end(), cmp);
    }

} // namespace filesystem
