#pragma once

#include <Eigen/Core>
#include <fstream>
#include <pangolin/pangolin.h>
#include <sophus/se3.hpp>
#include <sophus/sim3.hpp>
#include <sstream>

#include "util/HelperStructs.h"

namespace pcViewer {

    class KeyFrameData {
    public:
        KeyFrameData();

        // Setters
        void setTimestamp(uint64_t t_ns);

        void setWidth(float w);

        void setHeight(float h);

        void setExposure(double exposure_time);

        void setCameraIntrinsics(CameraIntrinsics &intr);

        void addPoint(Point<MAX_RES_PER_POINT> point);

        void setCamToWorld(const Sophus::SE3d &T_w_c);

        // Getters
        const uint64_t getTimestamp();

        const unsigned int getFrameId();

        const unsigned int getWidth();

        const unsigned int getHeight();

        const unsigned int getNumPoints();

        const double getExposure();

        std::vector<Point<MAX_RES_PER_POINT>> getPointCloud();

        const CameraIntrinsics &getCameraIntrinsics();

        const Sophus::SE3d &getCamToWorld();

    private:
        unsigned int frameId;
        static unsigned int nextId;
        uint64_t timestamp; // Timestamp in nanoseconds
        unsigned int width, height;
        double exposure_time_ms_;

        CameraIntrinsics cameraIntrinsics;

        std::vector<Point<MAX_RES_PER_POINT>> points;
        Sophus::SE3d camToWorld;
    };
} // namespace pcViewer
