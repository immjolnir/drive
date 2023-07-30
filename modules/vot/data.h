#pragma once

#include "config.h"
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
        Loader();

        ~Loader();

        bool load(const Sequence& sequence_config);

        std::vector<FramePtr> frames() const { return _frames; }

      private:
        /*
        We cannot construct or deconstruct it for:
        /usr/include/c++/11/bits/unique_ptr.h:83:23: error: invalid application of 'sizeof' to incomplete type
     'vot::Loader::Impl' 83 |         static_assert(sizeof(_Tp)>0,
        */
        std::unique_ptr<Impl> _impl;
        std::vector<FramePtr> _frames;
    };
}  // namespace vot
