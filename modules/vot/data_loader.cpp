#include <yaml-cpp/yaml.h>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>

namespace vot
{
    class DataLoader {};
}  // namespace vot

int main(int argc, char* argv[]) {
    std::string config_file(argv[1]);
    YAML::Node config = YAML::LoadFile(config_file);
    std::cout << "config=" << config << std::endl;

    YAML::Node sequences = config["sequences"];

    /*

    yaml 支持 Scalar、List、Map 类型，yaml-cpp 通过 NodeType 定义了 Node 的可能类型。
    namespace YAML
    {
        struct NodeType {
            enum value { Undefined, Null, Scalar, Sequence, Map };
        };
    }  // namespace YAML
    */
    std::cout << "Node type: " << config.Type() << std::endl;
    std::cout << "Node type: " << sequences.Type() << std::endl;

    std::map<std::string, YAML::Node> all;

    // yaml-cpp 中也可以通过迭代的方式，访问 Node 中的内容。
    for (YAML::const_iterator it = sequences.begin(); it != sequences.end(); ++it) {
        std::cout << "node value:" << *it << std::endl;
        std::cout << "<<< Node type: " << it->Type() << std::endl;
        const YAML::Node& node = *it;
        std::string dataset_name = node["name"].as<std::string>();
        all.emplace(dataset_name, node);
        for (YAML::const_iterator it = node.begin(); it != node.end(); ++it) {
            std::cout << "innermost node types] first: " << it->first.Type() << ", second: " << it->second.Type()
                      << std::endl;

            std::string key = it->first.as<std::string>();
            std::cout << "key=" << key << std::endl;
            std::cout << "const iterator: " << it->first << "=>" << it->second << std::endl;
        }
        // cout << it->first.as<string>() << ":" << it->second.as<int>() << endl;
    }

    YAML::Node bolt = sequences[0];
    std::cout << "bolt=" << bolt << std::endl;

    std::cout << "all.size=" << all.size() << std::endl;
    std::cout << "bolt=" << all["Bolt"] << std::endl;
}

// https://cloud.tencent.com/developer/article/1762157
// https://cloud.tencent.com/developer/article/1423468
// https://www.cnblogs.com/nbtech/p/use_yaml-cpp_library.html
// https://hackingcpp.com/cpp/libs/fmt.html#chars-width