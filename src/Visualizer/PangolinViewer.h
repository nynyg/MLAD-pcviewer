#pragma once

#include <deque>
#include <map>
#include <pangolin/pangolin.h>

#include "InputWrapper/KeyFrameData.h"
#include "KeyFrameDisplay.h"

namespace pcViewer {

    class PangolinViewer {
    public:
        PangolinViewer(int w, int h);

        void run();

        void publishKeyframes(std::vector<std::shared_ptr<KeyFrameData>> &kfs);

        void updateSettings();

    private:
        void drawTraj();

        int w, h;

        // 3D model rendering
        std::vector<std::unique_ptr<KeyFrameDisplay>> kfDisplay;
    };

} // namespace pcViewer
