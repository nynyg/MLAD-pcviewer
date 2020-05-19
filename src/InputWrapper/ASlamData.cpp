#include "ASlamData.h"

namespace pcViewer {

    ASlamData::ASlamData() {
        keyFrames.reserve(10000);
    }

    void ASlamData::addKeyframe(const std::shared_ptr<KeyFrameData> &kf) {
        keyFrames.emplace_back(kf);
    }

    std::vector<std::shared_ptr<KeyFrameData>> &ASlamData::getKeyFrames() {
        return keyFrames;
    }

} // namespace pcViewer