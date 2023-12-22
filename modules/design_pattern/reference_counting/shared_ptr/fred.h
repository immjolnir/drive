#pragma once

/** Reference Counting in C++
 * https://stackoverflow.com/questions/11788187/reference-counting-in-c-oo-style
 * https://yosefk.com/c++fqa/heap.html#fqa-16.26
 * 
 * using shared_ptr instead of reinventing the wheel (in a non-reusable way).
 */

#include <memory>
#include <string>
#include <utility>

// I don't think it's a good idea to reinvent reference counting especially since we have std::shared_ptr now in C++11.
// You can easily implement your possibly polymorphic reference-counted Pimpl idiom class in terms of std::shared_ptr.
// Notice how we don't have to implement copy ctor, assignment, dtor anymore and mutation gets simpler w.r.t. the
// reference counter and cloning:
class Fred {
  public:
    static Fred create1(std::string const& s, int i);
    static Fred create2(float x, float y);

    void sampleInspectorMethod() const;  // No changes to this object
    void sampleMutatorMethod();          // Change this object

    long int use_count() const { return data_.use_count(); }

    Fred clone() const;  // Deep Copy

    // No copy ctor
    // No assignment operators
    // No dtor
    // No movable semantic

    class Data;

  private:
    std::shared_ptr<Data> data_;

    explicit Fred(std::shared_ptr<Data> d) : data_(std::move(d)) {}
};

// I think reference counting, if you implement it yourself, is easier to get wrong.
// It also has the reputation of being slow in multithreaded environments because
// the reference counters have to be incremented and decremented atomically.
// But I guess due to C++11 which offers shared_ptr and move semantics,
// this copy-on-write pattern might get a bit more popular again.
// If you enable move semantics for the Fred class you can avoid some of the costs of atomically incrementing reference
// counters. So moving a Fred object from one location to another should be even faster than copying it.