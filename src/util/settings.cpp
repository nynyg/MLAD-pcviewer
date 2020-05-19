#include "util/settings.h"

namespace pcViewer {

namespace viewerSettings {
bool showKFCameras = false;
bool showCamera = false;
bool showTrajectory = false;
float scaledVarTH = 0.001;
float absVarTH = 0.001;
float minRelBS = 0.1;
int sparsity = 1;
float pointSize = 1.0;
float camSize = 0.3;
// color arrays
float blue[3] = {0, 0, 1};
float yellow[3] = {1, 1, 0};
float red[3] = {1, 0, 0};
} // namespace viewerSettings

} // namespace pcViewer
