#include "KeyFrameData.h"

namespace pcViewer {

    unsigned int KeyFrameData::nextId = 0;

    KeyFrameData::KeyFrameData() {
        frameId = nextId++;
        points.reserve(1000);
    }

    const unsigned int KeyFrameData::getFrameId() { return frameId; }

    void KeyFrameData::setTimestamp(uint64_t t_ns) { timestamp = t_ns; }

    const uint64_t KeyFrameData::getTimestamp() { return timestamp; }

    void KeyFrameData::setWidth(float w) { width = w; }

    const unsigned int KeyFrameData::getWidth() { return width; }

    void KeyFrameData::setHeight(float h) { height = h; }

    const unsigned int KeyFrameData::getHeight() { return height; }

    void KeyFrameData::setExposure(double exposure_time) {
        exposure_time_ms_ = exposure_time;
    }

    const double KeyFrameData::getExposure() { return exposure_time_ms_; }

    void KeyFrameData::setCameraIntrinsics(pcViewer::CameraIntrinsics &intr) {
        cameraIntrinsics.fx = intr.fx;
        cameraIntrinsics.fy = intr.fy;
        cameraIntrinsics.cx = intr.cx;
        cameraIntrinsics.cy = intr.cy;
    }

    const CameraIntrinsics &KeyFrameData::getCameraIntrinsics() {
        return cameraIntrinsics;
    }

    void KeyFrameData::addPoint(pcViewer::Point<MAX_RES_PER_POINT> point) {
        points.emplace_back(point);
    }

    const unsigned int KeyFrameData::getNumPoints() { return points.size(); }

    std::vector<Point<MAX_RES_PER_POINT>> KeyFrameData::getPointCloud() {
        return points;
    }

    void KeyFrameData::setCamToWorld(const Sophus::SE3d &T_w_c) {
        camToWorld = Sophus::SE3d{T_w_c.matrix()};
    }

    const Sophus::SE3d &KeyFrameData::getCamToWorld() {
        return camToWorld;
    }

} // namespace pcViewer