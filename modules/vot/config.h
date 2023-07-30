#pragma once

#include <map>
#include <string>

namespace vot
{
    struct Sequence {
        std::string name;
        std::string location;
        std::string image_folder;
        std::string image_filename_pattern;
        std::string groundtruth_rect;
        friend std::ostream& operator<<(std::ostream& out, const Sequence& seq);
    };

    class Config {
      public:
        bool load(const std::string& file);
        /**
         * @param second: configuration loading status. true: Success.
         */
        std::pair<const Sequence&, bool> get(const std::string& sequence_name);

      private:
        std::map<std::string, Sequence> _sequences;
    };

}  // namespace vot