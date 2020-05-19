#include "DataParser.h"
#include "util/Filesystem.h"
#include "util/HelperStructs.h"

namespace pcViewer {

    std::unique_ptr<ASlamData>
    DataParser::getSlamData(const std::string &dirPath) {
        slamData = std::make_unique<ASlamData>();
        populateData(dirPath);
        return std::move(slamData);
    }

    void DataParser::populateData(const std::string &dirPath) {
        std::string keyframeFolder = dirPath + "/KeyFrameData";
        std::vector<std::string> kfFiles;
        filesystem::getDirContent(keyframeFolder, kfFiles);
        for (std::size_t i = 0; i < kfFiles.size(); ++i) {
            std::cout << "\rParsing keyframes " << i + 1 << "/"
                      << kfFiles.size() << "...";
            std::cout.flush();
            parseKeyframeFile(kfFiles[i]);
        }
        std::cout << "\rParsing keyframes " << kfFiles.size() << "/" << kfFiles.size() << "... done"
                  << std::endl;
    }

    void DataParser::parseKeyframeFile(const std::string &filePath) {
        std::ifstream infile;
        infile.open(filePath.c_str(), std::ios::in);
        assert(infile.good());

        std::shared_ptr<KeyFrameData> kfData = std::make_shared<KeyFrameData>();
        unsigned int numPoints;

        std::string l1;
        std::getline(infile, l1);
        l1.clear();
        std::getline(infile, l1);

        {
            uint64_t timestamp;
            std::sscanf(l1.c_str(), "%lu", &timestamp);
            kfData->setTimestamp(timestamp);
        }

        {
            l1.clear();
            std::getline(infile, l1);
            l1.clear();
            std::getline(infile, l1);
            l1.clear();
            std::getline(infile, l1);
            CameraIntrinsics cameraIntrinsics;
            unsigned int width, height;
            if (std::sscanf(l1.c_str(), "%f,%f,%f,%f,%d,%d,%d",
                            &cameraIntrinsics.fx,
                            &cameraIntrinsics.fy, &cameraIntrinsics.cx,
                            &cameraIntrinsics.cy, &width, &height,
                            &numPoints) == 7) {
            } else {
                std::cerr<<"Failed to read camera intrinsics!\n";
                infile.close();
                return;
            }
            kfData->setWidth(width);
            kfData->setHeight(height);
            kfData->setCameraIntrinsics(cameraIntrinsics);
        }

        {
            double t1, t2, t3, x, y, z, w;
            l1.clear();
            std::getline(infile, l1);
            l1.clear();
            std::getline(infile, l1);
            l1.clear();
            std::getline(infile, l1);
            std::sscanf(l1.c_str(), "%lf,%lf,%lf,%lf,%lf,%lf,%lf", &t1,
                            &t2, &t3,
                            &x, &y, &z, &w);
            Eigen::Quaterniond q(w, x, y, z);
            Eigen::Matrix<double, 3, 1> trans(t1, t2, t3);
            Sophus::SE3d cam2World(q, trans);
            kfData->setCamToWorld(cam2World);
        }

        {
            l1.clear();
            std::getline(infile, l1);
            l1.clear();
            std::getline(infile, l1);
            l1.clear();
            std::getline(infile, l1);
            double exposure_time;
            std::sscanf(l1.c_str(), "%lf", &exposure_time);
            kfData->setExposure(exposure_time);
        }

        {
            l1.clear();
            std::getline(infile, l1);
            l1.clear();
            std::getline(infile, l1);

            Point<MAX_RES_PER_POINT> pc;

            for (size_t i = 0; i < numPoints; i++) {
                l1.clear();
                std::getline(infile, l1);
                Point<MAX_RES_PER_POINT> pc;
                std::sscanf(l1.c_str(), "%f,%f,%f,%f,%f,%d,%d", &pc.u, &pc.v,
                            &pc.idepth,
                            &pc.idepth_hessian, &pc.relObsBaseline,
                            &pc.numGoodRes,
                            &pc.status);
                l1.clear();
                std::getline(infile, l1);
                std::sscanf(l1.c_str(), "%d,%d,%d,%d,%d,%d,%d,%d,",
                            &pc.color[0],
                            &pc.color[1], &pc.color[2], &pc.color[3],
                            &pc.color[4],
                            &pc.color[5], &pc.color[6], &pc.color[7]);

                kfData->addPoint(pc);
            }
        }
        slamData->addKeyframe(kfData);

        infile.close();
    }

} // namespace pcViewer