#include <gtest/gtest.h>

// https://en.cppreference.com/w/cpp/memory/destroy
// gives an example of why this might be needed. In this case we have used placement new into a buffer and there is no
// need to free the memory:

#include <memory>  // std::destroy
#include <new>     // std::launder

struct Tracer {
    int value;

    ~Tracer() { std::cout << value << " destructed\n"; }
};

TEST(new_api, destroy) {
    alignas(Tracer) unsigned char buffer[sizeof(Tracer) * 8];

    for (int i = 0; i < 8; ++i) {
        new (buffer + sizeof(Tracer) * i) Tracer{i};  // manually construct objects
    }

    auto ptr = std::launder(reinterpret_cast<Tracer*>(buffer));

    std::destroy(ptr, ptr + 8);

    // https://en.cppreference.com/w/cpp/memory/destroy_at
    // for (int i = 0; i < 8; ++i) std::destroy_at(ptr + i);
}