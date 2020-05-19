#pragma once

#include "InputWrapper/ASlamData.h"
#include "InputWrapper/KeyFrameData.h"

namespace pcViewer {

    class DataParser {
    public:
        // Getters
        std::unique_ptr<ASlamData> getSlamData(const std::string &dirPath);

    private:
        void populateData(const std::string &dirPath);

        // Data Parser APIs
        void parseKeyframeFile(const std::string &filePath);

        std::unique_ptr<ASlamData> slamData;
    };

} // namespace pcViewer
