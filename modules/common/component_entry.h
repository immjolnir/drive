#pragma once

#include <boost/noncopyable.hpp>

#include <string>

// Entrypoint for simple cli (non-ros) using programs.
class ComponentEntry : private boost::noncopyable {
  public:
    ComponentEntry(const std::string& component_name) : _component_name(component_name) {}

    virtual ~ComponentEntry() = default;

    int run(int argc, char** argv);

  private:
    std::string _component_name;
};
