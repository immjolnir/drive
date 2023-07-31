#include "data.h"

#include <glog/logging.h>
#include <boost/filesystem.hpp>

#include <cassert>
#include <fstream>  // ifstream
#include <vector>

namespace vot
{
    // Internal class: Impl
    class Loader::Impl {
      public:
        bool load(const Config::Sequence& conf, std::vector<FramePtr>& frames) {
            boost::filesystem::path location(conf.location);
            location /= conf.name;

            if (!boost::filesystem::exists(location)) {  // does location acutally exist?
                LOG(ERROR) << "No such file: " << location;
                return false;
            }
            if (boost::filesystem::is_regular_file(location)) {
                LOG(ERROR) << "Found a regular file: " << location;
                // is location a regular file?
                return false;
            }
            if (!boost::filesystem::is_directory(location)) {
                LOG(ERROR) << "Found a not-directory: " << location;
                return false;
            }
            boost::filesystem::path images_path = location / conf.image_folder;
            if (!boost::filesystem::exists(images_path)) {
                LOG(ERROR) << "No such file: " << location;
                return false;
            }

            std::vector<boost::filesystem::directory_entry> images_dirs;
            std::copy(boost::filesystem::directory_iterator(images_path), boost::filesystem::directory_iterator(),
                      std::back_inserter(images_dirs));

            std::sort(images_dirs.begin(), images_dirs.end());

            for (std::vector<boost::filesystem::directory_entry>::const_iterator it = images_dirs.begin();
                 it != images_dirs.end(); ++it) {
                LOG(ERROR) << "found path: " << it->path().string();
            }

            // groundtruth rect
            boost::filesystem::path groundtruth_rect = location / conf.groundtruth_rect;
            if (!boost::filesystem::is_regular_file(groundtruth_rect)) {
                LOG(ERROR) << groundtruth_rect << " is not a regular file";
                return false;
            }
            // boost::filesytem::ifstream is just another input stream.
            // and all standard algorithms apply to it the same way they apply to std::ifstream.
            // std::getline works with it as well
            // boost::filesystem::ifstream fileHandler(groundtruth_rect.string());
            std::ifstream groundtruth_istream;
            // open a file to perform read operation using file object.
            groundtruth_istream.open(groundtruth_rect.string(), std::ios::in);

            std::vector<BBox> bboxes;
            // Checking whether the file is open.
            if (groundtruth_istream.is_open()) {
                std::string line;
                // Read data from the file object and put it into a string
                while (std::getline(groundtruth_istream, line)) {
                    std::istringstream s(line);

                    std::vector<float> numbers;
                    float x;
                    char delimiter;
                    while (s >> x) {
                        numbers.emplace_back(x);
                        s >> delimiter;
                    }
                    assert(numbers.size() == 4);
                    BBox bbox;
                    if (numbers.size() == 4) {
                        float x = numbers[0], y = numbers[1], w = numbers[2], h = numbers[3];
                        bbox.x = x;
                        bbox.y = y;
                        bbox.width = w;
                        bbox.height = h;
                    }

                    bboxes.emplace_back(std::move(bbox));
                }
            }
            LOG(ERROR) << "bboxes.size=" << bboxes.size() << ", images.size=" << images_dirs.size();

            assert(bboxes.size() == images_dirs.size());
            assert(frames.empty());
            // Avoid reallocation
            frames.reserve(bboxes.size());

            for (size_t i = 0; i < bboxes.size(); ++i) {
                auto frame = std::make_shared<Frame>();
                frame->image = cv::imread(images_dirs[i].path().string());
                frame->groundtruth = bboxes[i];
                frames.emplace_back(frame);
            }
            LOG(ERROR) << "frames.size=" << frames.size() << ", images.size=" << images_dirs.size();

            assert(frames.size() == bboxes.size());

            return true;
        }
    };

    Loader::Loader() { _impl = std::make_unique<Impl>(); }

    Loader::~Loader() {}

    bool Loader::load(const Config::Sequence& sequence_config) {
        if (_impl) {
            return _impl->load(sequence_config, _frames);
        }
        return false;
    }

}  // namespace vot
