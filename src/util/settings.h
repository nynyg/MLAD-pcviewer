#pragma once

#include <cmath>
#include <string.h>
#include <string>

namespace pcViewer {
// ============== PARAMETERS TO BE DECIDED ON COMPILE TIME =================
    namespace viewerSettings {
        extern bool showKFCameras;
        extern bool showCamera;
        extern bool showTrajectory;
        extern float scaledVarTH;
        extern float absVarTH;
        extern float minRelBS;
        extern int sparsity;
        extern float pointSize;
        extern float camSize;
// color arrays
        extern float blue[3];
        extern float yellow[3];
        extern float red[3];
    }; // namespace viewerSettings

} // namespace pcViewer
