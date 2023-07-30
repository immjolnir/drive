#include "data.h"

namespace vot
{
    // Internal class: Impl
    class Loader::Impl {
      public:
        bool load(const Sequence& sequence_config, std::vector<FramePtr>& frames) { return false; }
    };

    Loader::Loader() { _impl = std::make_unique<Impl>(); }
    Loader::~Loader() {}
    bool Loader::load(const Sequence& sequence_config) {
        if (_impl) {
            return _impl->load(sequence_config, _frames);
        }
        return false;
    }

}  // namespace vot
