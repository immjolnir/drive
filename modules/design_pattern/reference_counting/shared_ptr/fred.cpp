
#include "fred.h"

#include <cassert>
#include <iostream>

using std::shared_ptr;

class Fred::Data {
  public:
    virtual ~Data() { std::cout << "~Data()" << std::endl; }  // A virtual destructor

    virtual shared_ptr<Data> clone() const = 0;      // A virtual constructor
    virtual void sampleInspectorMethod() const = 0;  // A pure virtual function
    virtual void sampleMutatorMethod() = 0;
};

namespace {

class Der1 : public Fred::Data {
  public:
    Der1(std::string const& s, int i);
    Der1(const Der1& obj);

    virtual ~Der1() { std::cout << "~Der1()" << std::endl; };  // Best practice, 添加 virtual dtor

    virtual shared_ptr<Data> clone() const override;
    virtual void sampleInspectorMethod() const override;
    virtual void sampleMutatorMethod() override;
    // ...
  private:
    std::string s_;
    int i_;
};

// insert Der1 function definitions here
Der1::Der1(std::string const& s, int i) : s_(s), i_(i) {}

Der1::Der1(const Der1& obj) {
    std::cout << "Der1(const Der1&)" << std::endl;
    this->s_ = obj.s_;
    this->i_ = obj.i_ + 1;  // hacking it to show the difference
}

shared_ptr<Fred::Data> Der1::clone() const {
    // Create an object that is owned by a shared_ptr.
    // A shared_ptr that owns the newly created object
    // It requires the underlying class has copy ctor.
    auto ptr = std::make_shared<Der1>(*this);
    return ptr;
}

void Der1::sampleInspectorMethod() const {
    std::cout << "Der1::sampleInspectorMethod(): s=" << s_ << ", i=" << i_ << std::endl;
}

void Der1::sampleMutatorMethod() { std::cout << "Der1::sampleMutatorMethod(): s=" << s_ << ", i=" << i_ << std::endl; }

class Der2 : public Fred::Data {
  public:
    Der2(float x, float y);
    Der2(const Der2& obj);

    virtual ~Der2() { std::cout << "~Der2()" << std::endl; };  // Best practice, 添加 virtual dtor

    virtual shared_ptr<Fred::Data> clone() const override;
    virtual void sampleInspectorMethod() const override;
    virtual void sampleMutatorMethod() override;
    // ...
  private:
    float x_;
    float y_;
};

// insert Der2 function definitions here

Der2::Der2(float x, float y) : x_(x), y_(y) {}

Der2::Der2(const Der2& obj) {
    std::cout << "Der2(const Der2&)" << std::endl;
    this->x_ = obj.x_;
    this->y_ = obj.y_ + 1;  // hacking it to show the difference
}

shared_ptr<Fred::Data> Der2::clone() const {
    auto ptr = std::make_shared<Der2>(*this);
    return ptr;
}

void Der2::sampleInspectorMethod() const {
    std::cout << "Der2::sampleInspectorMethod(): x=" << x_ << ", y=" << y_ << std::endl;
}

void Der2::sampleMutatorMethod() {
    std::cout << "Der2::sampleInspectorMethod(): x=" << x_ << ", y=" << y_ << std::endl;
}

}  // namespace

Fred Fred::create1(std::string const& s, int i) { return Fred(std::make_shared<Der1>(s, i)); }

Fred Fred::create2(float x, float y) { return Fred(std::make_shared<Der2>(x, y)); }

Fred Fred::clone() const { return Fred(this->data_->clone()); }

void Fred::sampleInspectorMethod() const {
    // This method promises ("const") not to change anything in *data_
    // Therefore we simply "pass the method through" to *data_:
    data_->sampleInspectorMethod();
}

void Fred::sampleMutatorMethod() {
    // This method might need to change things in *data_
    // Thus it first checks if this is the only pointer to *data_
    if (!data_.unique()) data_ = data_->clone();
    assert(data_.unique());

    // Now we "pass the method through" to *data_:
    data_->sampleMutatorMethod();
}