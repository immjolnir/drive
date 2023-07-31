#include "common/component_entry.h"
#include "config.h"
#include "data.h"

#include <glog/logging.h>

using namespace vot;

DEFINE_string(conf, "conf.yaml", "vot sequences conf");
DEFINE_string(sequence, "Bolt", "the name of a sequence data.");

class VOTProgram : public ComponentEntry {
  public:
    VOTProgram() : ComponentEntry("VOTProgram") {}

    virtual ~VOTProgram() = default;

    int run(int argc, char** argv) {
        ComponentEntry::run(argc, argv);

        Config config;
        if (!config.load(FLAGS_conf)) {
            LOG(ERROR) << "Failed to load the " << FLAGS_conf;
            return 1;
        }

        auto ret = config.get(FLAGS_sequence);
        if (!ret.second) {
            LOG(ERROR) << "Failed to get the sequence: " << FLAGS_sequence;
            return 2;
        }

        auto sequence = ret.first;
        if (!_loader.load(sequence)) {
            LOG(ERROR) << "Failed to load the sequence: " << sequence;
            return 3;
        }

        auto frames = _loader.frames();
        assert(!frames.empty());
        for (const auto& frame : frames) {
            const auto& gt_box = frame->groundtruth;
            cv::Rect rect = cv::Rect(cv::Point(int(gt_box.x), int(gt_box.y)), cv::Size(gt_box.width, gt_box.height));

            cv::Mat I = frame->image.clone();
            cv::rectangle(I, rect, cv::Scalar(0, 255, 0), 2);
            cv::imshow("vot", I);
            cv::waitKey();
        }

        return 0;
    }

  private:
    Loader _loader;
};

int main(int argc, char* argv[]) {
    VOTProgram prog;
    return prog.run(argc, argv);
}