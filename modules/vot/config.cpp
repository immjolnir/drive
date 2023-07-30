#include "config.h"

#include <glog/logging.h>
#include <yaml-cpp/yaml.h>

#include <cassert>
#include <map>
#include <ostream>
#include <string>

namespace vot
{

    std::ostream& operator<<(std::ostream& out, const Sequence& seq) {
        out << "Sequence [ name:" << seq.name << ", location=" << seq.location << ", image_folder=" << seq.image_folder
            << ", image_filename_pattern=" << seq.image_filename_pattern
            << ", groundtruth_rect=" << seq.groundtruth_rect << "]";
        return out;
    };

    bool Config::load(const std::string& file) {
        // TODO: what's the error when loading failed.
        YAML::Node config = YAML::LoadFile(file);
        std::string location = config["location"].as<std::string>();
        YAML::Node sequences = config["sequences"];

        for (YAML::const_iterator it = sequences.begin(); it != sequences.end(); ++it) {
            const YAML::Node& node = *it;
            Sequence instance;
            instance.name = node["name"].as<std::string>();
            instance.image_folder = node["image_folder"].as<std::string>();
            instance.image_filename_pattern = node["image_filename_pattern"].as<std::string>();
            instance.groundtruth_rect = node["groundtruth_rect"].as<std::string>();

            auto ret = _sequences.emplace(instance.name, std::move(instance));
            if (!ret.second) {
                LOG(ERROR) << "emplace failed, the exsting value: " << ret.first->second;
            }
            assert(ret.second);
        }

        return true;
    }

    std::pair<const Sequence&, bool> Config::get(const std::string& sequence_name) {
        static const Sequence empty;
        auto it = _sequences.find(sequence_name);
        if (it != _sequences.end()) {
            return {it->second, true};
        } else {
            return {empty, false};
        }
    }
}  // namespace vot
