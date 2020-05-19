#include "DataParser/DataParser.h"
#include "InputWrapper/ASlamData.h"
#include "Visualizer/PangolinViewer.h"
#include "util/Filesystem.h"
#include "util/settings.h"
#include <fstream>

using namespace pcViewer;

std::string source = "";

void parseArgument(char *arg) {
    char buf[1000];

    if (1 == sscanf(arg, "data=%s", buf)) {
        source = buf;
        std::cout << "Load Keyframe data from " << source << std::endl;
        return;
    }

    std::cout << "could not parse argument " << arg << std::endl;
}

int main(int argc, char **argv) {
    int w = 800, h = 400;

    for (int i = 1; i < argc; i++)
        parseArgument(argv[i]);

    std::unique_ptr<DataParser> reader(new DataParser());
    std::unique_ptr<ASlamData> slamData = std::move(
            reader->getSlamData(source));

    std::unique_ptr<PangolinViewer> viewer(new PangolinViewer(w, h));
    viewer->publishKeyframes(slamData->getKeyFrames());

    viewer->run();

    return 0;
}
