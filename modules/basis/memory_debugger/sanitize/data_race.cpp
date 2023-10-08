#include <thread>

static volatile bool flip1{false};
static volatile bool flip2{false};

int main() {
    std::thread t([&]() {
        while (!flip1)
            ;
        flip2 = true;
    });

    flip1 = true;
    while (!flip2)
        ;
    t.join();
}