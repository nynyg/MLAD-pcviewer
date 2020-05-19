#include "PangolinViewer.h"
#include "KeyFrameDisplay.h"
#include "util/settings.h"

namespace pcViewer {

// pangolin gui parameter list
    pangolin::Var<bool> settings_showKFCameras("ui.KFCam", true, true);
    pangolin::Var<bool> settings_showTrajectory("ui.Trajectory", true, true);
    pangolin::Var<int> settings_sparsity("ui.sparsity", 1, 1, 20, false);
    pangolin::Var<double> settings_scaledVarTH("ui.relVarTH", 0.001, 1e-10,
                                               1e10,
                                               true);
    pangolin::Var<double> settings_absVarTH("ui.absVarTH", 0.001, 1e-10, 1e10,
                                            true);
    pangolin::Var<double> settings_minRelBS("ui.minRelativeBS", 0.1, 0, 1,
                                            false);
    pangolin::Var<double> settings_pointSize("ui.pointSize", 1.0, 0.5, 2,
                                            false);
    pangolin::Var<double> settings_camSize("ui.camSize", 0.3, 0.1, 1,
                                             false);

    PangolinViewer::PangolinViewer(int w, int h) {
        this->w = w;
        this->h = h;
        this->kfDisplay.reserve(10000);
    }

    void PangolinViewer::run() {
        std::cout << "Start Viewer!" << std::endl;

        pangolin::CreateWindowAndBind("Main", 2 * w, 2 * h);

        const int UI_WIDTH = 180;

        glEnable(GL_DEPTH_TEST);

        // 3D visualization
        pangolin::OpenGlRenderState Visualization3D_camera(
                pangolin::ProjectionMatrix(w, h, 400, 400, w / 2, h / 2, 0.1,
                                           10000),
                pangolin::ModelViewLookAt(-0, 5, 100, 0, 0, 0,
                                          pangolin::AxisNegY));

        pangolin::View &Visualization3D_display =
                pangolin::CreateDisplay().SetBounds(0.0, 1.0, 0, 1.0,
                                                    -w / (float) h);
        pangolin::Handler3D *handler =
                new pangolin::Handler3D(Visualization3D_camera);
        Visualization3D_display.SetHandler(handler);

        // parameter reconfigure gui
        pangolin::CreatePanel("ui").SetBounds(0.0, 1.0, 0.0,
                                              pangolin::Attach::Pix(UI_WIDTH));

        // update parameters
        updateSettings();

        while (!pangolin::ShouldQuit()) {
            // Clear entire screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // Set the background color of the Pangolin viewer to white
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

                // Activate efficiently by object
                Visualization3D_display.Activate(Visualization3D_camera);
                int refreshed = 0;
                for (auto &kfD : kfDisplay) {
                    if (viewerSettings::showKFCameras) {
                        kfD->drawCam(2, viewerSettings::blue, viewerSettings::camSize);
                    }

                    refreshed += (int) (kfD->refreshPC(refreshed < 10));
                    kfD->drawPC(viewerSettings::pointSize);
                }

                if (viewerSettings::showCamera) {
                    size_t nKeyframes = kfDisplay.size();
                    kfDisplay[0]->drawCam(2, viewerSettings::red, viewerSettings::camSize);
                    kfDisplay[nKeyframes - 1]->drawCam(2, viewerSettings::red,
                                                       0.1);
                }
                if (viewerSettings::showKFCameras)
                    drawTraj();

            updateSettings();

            // Swap frames and Process Events
            pangolin::FinishFrame();
        }

        std::cout << "Quit Viewer." << std::endl;
        exit(1);
    }

    void PangolinViewer::drawTraj() {

        if (viewerSettings::showTrajectory) {
            glColor3f(viewerSettings::red[0], viewerSettings::red[1],
                      viewerSettings::red[2]);
            glLineWidth(3);

            glBegin(GL_LINE_STRIP);
            for (unsigned int i = 0; i < kfDisplay.size(); i++) {
                glVertex3f((float) kfDisplay[i]->getKeyframePos()[0],
                           (float) kfDisplay[i]->getKeyframePos()[1],
                           (float) kfDisplay[i]->getKeyframePos()[2]);
            }
            glEnd();
        }
    }

    void PangolinViewer::publishKeyframes(
            std::vector<std::shared_ptr<KeyFrameData>> &kfs) {
        for (auto &kf : kfs) {
            kfDisplay.emplace_back(std::make_unique<KeyFrameDisplay>(kf));
        }
    }

    void PangolinViewer::updateSettings() {

        viewerSettings::showKFCameras = settings_showKFCameras.Get();
        viewerSettings::showTrajectory = settings_showTrajectory.Get();
        viewerSettings::absVarTH = settings_absVarTH.Get();
        viewerSettings::scaledVarTH = settings_scaledVarTH.Get();
        viewerSettings::minRelBS = settings_minRelBS.Get();
        viewerSettings::sparsity = settings_sparsity.Get();
        viewerSettings::pointSize = settings_pointSize.Get();
        viewerSettings::camSize = settings_camSize.Get();
    }

} // namespace pcViewer
