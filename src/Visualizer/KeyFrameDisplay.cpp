#include "KeyFrameDisplay.h"
#include "util/settings.h"
#include <pangolin/pangolin.h>
#include <stdio.h>

namespace pcViewer {

    KeyFrameDisplay::KeyFrameDisplay(const std::shared_ptr<KeyFrameData> &kf) {
        kfData = kf;

        needRefresh = true;

        my_scaledTH = 0.001;
        my_absTH = 0.001;
        my_minRelBS = 0.1;
        my_sparsifyFactor = 1;

        numGLBufferPoints = 0;
        bufferValid = false;

        my_camSize = 0.3;
        my_pointSize = 1.0;
    }

    bool KeyFrameDisplay::refreshPC(bool canRefresh) {
        if (canRefresh) {
            needRefresh = needRefresh ||
                          my_scaledTH != viewerSettings::scaledVarTH ||
                          my_absTH != viewerSettings::absVarTH ||
                          my_minRelBS != viewerSettings::minRelBS ||
                          my_camSize != viewerSettings::camSize ||
                          my_pointSize != viewerSettings::pointSize ||
                          my_sparsifyFactor != viewerSettings::sparsity;
        }

        if (!needRefresh)
            return false;
        needRefresh = false;

        my_scaledTH = viewerSettings::scaledVarTH;
        my_absTH = viewerSettings::absVarTH;
        my_minRelBS = viewerSettings::minRelBS;
        my_sparsifyFactor = viewerSettings::sparsity;
        my_camSize = viewerSettings::camSize;
        my_pointSize = viewerSettings::pointSize;

        // if there are no vertices, done!
        if (kfData->getNumPoints() == 0)
            return false;

        unsigned int vertexBufferNumPoints = 0;
        // make data
        std::vector<Vec3f> tmpVertexBuffer;
        std::vector<Vec3b> tmpColorBuffer;

        const unsigned int numPoints = kfData->getNumPoints();
        std::vector<Point<MAX_RES_PER_POINT>> points = kfData->getPointCloud();
        const CameraIntrinsics cameraIntrinsics = kfData->getCameraIntrinsics();
        double fxi = 1 / cameraIntrinsics.fx;
        double fyi = 1 / cameraIntrinsics.fy;
        double cxi = -cameraIntrinsics.cx / cameraIntrinsics.fx;
        double cyi = -cameraIntrinsics.cy / cameraIntrinsics.fy;

        // make data
        tmpVertexBuffer.resize(numPoints);
        tmpColorBuffer.resize(numPoints);

        for (size_t i = 0; i < numPoints; i++) {

            if (points[i].idepth < 0)
                continue;

            float depth = 1.0f / points[i].idepth;
            float depth4 = depth * depth;
            depth4 *= depth4;
            float var = (1.0f / (points[i].idepth_hessian + 0.01));

            if (var * depth4 > my_scaledTH)
                continue;

            if (var > my_absTH)
                continue;

            if (points[i].relObsBaseline < my_minRelBS)
                continue;

            // randomly remove points to sparsify
            if (my_sparsifyFactor > 1 && rand() % my_sparsifyFactor != 0)
                continue;

            tmpVertexBuffer[vertexBufferNumPoints][0] =
                    ((points[i].u) * fxi + cxi) * depth;
            tmpVertexBuffer[vertexBufferNumPoints][1] =
                    ((points[i].v) * fyi + cyi) * depth;
            tmpVertexBuffer[vertexBufferNumPoints][2] =
                    depth *
                    (1 + 2 * fxi * (rand() / (float) RAND_MAX - 0.5f));

            tmpColorBuffer[vertexBufferNumPoints][0] =
                    points[i].color[4]; // 4th position refers to middle point
            tmpColorBuffer[vertexBufferNumPoints][1] = points[i].color[4];
            tmpColorBuffer[vertexBufferNumPoints][2] = points[i].color[4];

            vertexBufferNumPoints++;

            assert(vertexBufferNumPoints <= numPoints);
        }


        if (vertexBufferNumPoints == 0) {
            return true;
        }

        numGLBufferGoodPoints = vertexBufferNumPoints;
        if (numGLBufferGoodPoints > numGLBufferPoints) {
            numGLBufferPoints = vertexBufferNumPoints * 1.3;
            vertexBuffer.Reinitialise(pangolin::GlArrayBuffer,
                                      numGLBufferPoints,
                                      GL_FLOAT, 3, GL_DYNAMIC_DRAW);
            colorBuffer.Reinitialise(pangolin::GlArrayBuffer,
                                     numGLBufferPoints,
                                     GL_UNSIGNED_BYTE, 3, GL_DYNAMIC_DRAW);
        }
        vertexBuffer.Upload(tmpVertexBuffer.data(),
                            sizeof(float) * 3 * numGLBufferGoodPoints, 0);
        colorBuffer.Upload(tmpColorBuffer.data(),
                           sizeof(unsigned char) * 3 * numGLBufferGoodPoints,
                           0);
        bufferValid = true;

        return true;
    }

    void
    KeyFrameDisplay::drawCam(float lineWidth, float *color, float sizeFactor) {
        if (kfData->getWidth() == 0)
            return;

        float sz = sizeFactor;
        CameraIntrinsics cameraIntrinsics = kfData->getCameraIntrinsics();
        float cx = cameraIntrinsics.cx;
        float cy = cameraIntrinsics.cy;
        float fx = cameraIntrinsics.fx;
        float fy = cameraIntrinsics.fy;
        unsigned int width = kfData->getWidth();
        unsigned int height = kfData->getHeight();

        glPushMatrix();

        Sophus::Matrix4f m = kfData->getCamToWorld()
                .matrix()
                .cast<float>();

        glMultMatrixf((GLfloat *) m.data());

        if (color == 0) {
            glColor3f(0.0f, 0.0f, 1.0f);
        } else
            glColor3f(color[0], color[1], color[2]);

        glLineWidth(lineWidth);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(sz * (0 - cx) / fx, sz * (0 - cy) / fy, sz);
        glVertex3f(0, 0, 0);
        glVertex3f(sz * (0 - cx) / fx, sz * (height - 1 - cy) / fy, sz);
        glVertex3f(0, 0, 0);
        glVertex3f(sz * (width - 1 - cx) / fx, sz * (height - 1 - cy) / fy,
                   sz);
        glVertex3f(0, 0, 0);
        glVertex3f(sz * (width - 1 - cx) / fx, sz * (0 - cy) / fy, sz);

        glVertex3f(sz * (width - 1 - cx) / fx, sz * (0 - cy) / fy, sz);
        glVertex3f(sz * (width - 1 - cx) / fx, sz * (height - 1 - cy) / fy,
                   sz);

        glVertex3f(sz * (width - 1 - cx) / fx, sz * (height - 1 - cy) / fy,
                   sz);
        glVertex3f(sz * (0 - cx) / fx, sz * (height - 1 - cy) / fy, sz);

        glVertex3f(sz * (0 - cx) / fx, sz * (height - 1 - cy) / fy, sz);
        glVertex3f(sz * (0 - cx) / fx, sz * (0 - cy) / fy, sz);

        glVertex3f(sz * (0 - cx) / fx, sz * (0 - cy) / fy, sz);
        glVertex3f(sz * (width - 1 - cx) / fx, sz * (0 - cy) / fy, sz);

        glEnd();
        glPopMatrix();
    }

    void KeyFrameDisplay::drawPC(float pointSize) {

        if (!bufferValid || numGLBufferGoodPoints == 0)
            return;

        glDisable(GL_LIGHTING);

        glPushMatrix();

        Sophus::Matrix4f m = kfData->getCamToWorld()
                .matrix()
                .cast<float>();

        glMultMatrixf((GLfloat *) m.data());

        glPointSize(pointSize);

        colorBuffer.Bind();
        glColorPointer(colorBuffer.count_per_element, colorBuffer.datatype, 0,
                       0);
        glEnableClientState(GL_COLOR_ARRAY);

        vertexBuffer.Bind();
        glVertexPointer(vertexBuffer.count_per_element, vertexBuffer.datatype,
                        0, 0);
        glEnableClientState(GL_VERTEX_ARRAY);
        glDrawArrays(GL_POINTS, 0, numGLBufferGoodPoints);
        glDisableClientState(GL_VERTEX_ARRAY);
        vertexBuffer.Unbind();

        glDisableClientState(GL_COLOR_ARRAY);
        colorBuffer.Unbind();

        glPopMatrix();
    }

    Eigen::Matrix<double, 3, 1> KeyFrameDisplay::getKeyframePos() {
        // return
        // (kfData->getCamToEarth(viewerSettings::showGPSResult).translation());
        return (kfData->getCamToWorld().translation());
    }

} // namespace pcViewer
