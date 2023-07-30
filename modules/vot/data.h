#pragma once

#include "sequence.h"
#include "frame.h"

#include <memory>
#include <string>
#include <vector>

namespace vot
{
    using FramePtr = std::shared_ptr<Frame>;

    class Loader {
      private:
        // Declare the internal class for quicker compilation
        class Impl;

      public:
        Loader() { _impl = std::make_unique<Impl>(this); }

        ~Loader() = default;

        bool load(const Config& config);

        std::vector<FramePtr> frames() const { return _frames; }

      private:
        std::unique_ptr<Impl> _impl;
        std::vector<FramePtr> _frames;
    };
}  // namespace vot
