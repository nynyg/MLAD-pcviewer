#pragma once

#include <Eigen/Core>
#include <fstream>
#include <pangolin/pangolin.h>
#include <sophus/se3.hpp>
#include <sophus/sim3.hpp>
#include <sstream>

#include "InputWrapper/KeyFrameData.h"
#include "util/HelperStructs.h"
#include "util/settings.h"

namespace pcViewer {

// stores a pointcloud associated to a Keyframe.
    class KeyFrameDisplay {

    public:
        explicit KeyFrameDisplay(const std::shared_ptr<KeyFrameData> &kf);

        // copies & filters internal data to GL buffer for rendering. if nothing to
        // do: does nothing.
        bool refreshPC(bool canRefresh);

        // rendering APIs
        void drawPC(float pointSize); // renders pointcloud
        void drawCam(float lineWidth = 1, float *color = nullptr,
                     float sizeFactor = 1); // renders cam

        // getters
        Eigen::Matrix<double, 3, 1> getKeyframePos();

    private:
        std::shared_ptr<KeyFrameData> kfData;

        float my_scaledTH, my_absTH;
        int my_sparsifyFactor;
        float my_minRelBS;
        bool needRefresh;
        float my_camSize, my_pointSize;

        bool bufferValid;
        int numGLBufferPoints;
        int numGLBufferGoodPoints;
        pangolin::GlBuffer vertexBuffer;
        pangolin::GlBuffer colorBuffer;
    };

} // namespace pcViewer