#pragma once

#include "InputWrapper/KeyFrameData.h"
#include "util/Filesystem.h"
#include <iostream>
#include <sstream>
#include <vector>

namespace pcViewer {

    class ASlamData {

    public:
        ASlamData();

        // Getters
        std::vector<std::shared_ptr<KeyFrameData>> &getKeyFrames();

        // Setters
        void addKeyframe(const std::shared_ptr<KeyFrameData> &kf);

    private:
        // Data exported from one DSO run
        std::vector<std::shared_ptr<KeyFrameData>> keyFrames;
        Sophus::SE3d origin;
    };

} // namespace pcViewer
