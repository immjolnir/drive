/*
// https://stackoverflow.com/questions/65877299/wsign-conversion-error-when-getting-index-for-vector

    srand(time(NULL));

Note that you will likely receive a similar warning for your call to `srand()` – the time_t value returned by the time()
call is most often a signed type (though this isn't explicitly mentioned by the Standard, IIRC) but srand expects an
unsigned argument.

Further, as you're using C++, you should consider replacing your use of rand() with the far more versatile functions
provided by the STL, in the <random> header. With these, you can dispense with the need for the % 3 operation (by
specifying a range of 0 thru 2 to the generator) and also avoid the need for a cast (by specifying an unsigned type for
that generator).
*/
#ifdef USE_C
#include <ctime>  // Should really use "ctime" in place of "time.h" when using C++
#else
#include <chrono>
#endif

#include <iostream>
#include <random>
#include <vector>

int main() {
#ifdef USE_C
    std::mt19937 gen(static_cast<unsigned int>(std::time(nullptr)));  // Cast "time_t" to unsiged
#else
    // https://stackoverflow.com/questions/19665818/generate-random-numbers-using-c11-random-library
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 gen(static_cast<unsigned int>(seed));
#endif

    std::uniform_int_distribution<unsigned int> randoms(0, 2);  // Set type & range (see below)
    std::vector<std::string> possible_result{"Rock", "Paper", "Scissors"};
    std::string selection{};
    std::cout << "Please make your choice\n";
    std::cout << "(R)ock, (P)aper, (S)cissors: ";
    while (std::cin >> selection) {
        if (selection == "q") {
            break;
        } else if (selection != "R" && selection != "P" && selection != "S") {
            std::cout << "I didn't get that, try again.\n";
        } else {
            // In the call below, the random number is already "unsigned int"
            // and in the correct range...
            std::string election = possible_result[randoms(gen)];
            std::cout << election << '\n';
        }
        std::cout << "(R)ock, (P)aper, (S)cissors: ";
    }
    return 0;
}