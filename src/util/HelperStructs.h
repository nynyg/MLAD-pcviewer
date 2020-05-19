#pragma once

#include "Eigen/Core"

static constexpr int MAX_RES_PER_POINT = 8;

namespace pcViewer {

    template<int resolution>
    struct Point {
        float u;
        float v;
        float idepth;
        float idepth_hessian;
        float relObsBaseline;
        int color[resolution]{};
        int numGoodRes;
        int status;
    };

    struct CameraIntrinsics {
        float fx;
        float fy;
        float cx;
        float cy;

        CameraIntrinsics() : fx(0.0), fy(0.0), cx(0.0), cy(0.0) {}
    };

    typedef Eigen::Matrix<unsigned char, 3, 1> Vec3b;
    typedef Eigen::Matrix<float, 3, 1> Vec3f;
} // namespace pcViewer
