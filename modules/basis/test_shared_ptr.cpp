#include <gtest/gtest.h>

#include <iostream>
#include <memory>

using namespace std;

class Student {
  public:
    Student() { cout << "Student()" << endl; }

    // User-Provided Copy ctor
    Student(const Student&) { cout << "Student(const Student&)" << endl; }

    Student(const Student&&) { cout << "Student(const Student&&)" << endl; }

    // User-Provided Copy assignment operator
    Student operator=(const Student&) {
        cout << "operator=(const Student&)" << endl;
        Student st;
        return st;
    }
};

/** Deep Copy
 * https://stackoverflow.com/questions/19969260/stdshared-ptr-deep-copy-object
 *
If you want to make a copy of the Graph object when you make a copy of the object, you can always define your copy
constructor and assignment operator to do just that:

State::State(const State& rhs) : _graph(std::make_shared(*rhs._graph)) {
   // Handled by initializer list
}
State::State(State&& rhs) : _graph(std::move(rhs._graph)) {
   // Handled by initializer list
}
State& State::operator= (State rhs) {
    std::swap(*this, rhs);
    return *this;
}

https://stackoverflow.com/questions/22782288/how-to-properly-duplicate-an-object-given-its-shared-ptr

std::make_shared is just a simple template function that creates the objects, passing all arguments to the constructor :

template<class T, class... Args>
shared_ptr<T> make_shared(Args&&... args)
{
  return shared_ptr<T>( new T( std::forward<Args>( args )... ) );
}
In your particular case :

std::shared_ptr<Event> o = std::make_shared<Event>(*e);
the object is copied.

If your code is such :

void foo() {
    // create new object using default constructor
    std::shared_ptr<Event> e = std::make_shared<Event>();
    // create new object using copy constructor constructor
    std::shared_ptr<Event> o = std::make_shared<Event>(*e);
}
then of course both objects are destroyed, when they go out of scope.
*/
TEST(shared_ptr, return_a_ptr_owns_newly_created_obj) {
    shared_ptr<Student> global = make_shared<Student>();
    EXPECT_TRUE(global != nullptr);

    auto ptr_copy = global;
    EXPECT_TRUE(ptr_copy == global);

    auto another = make_shared<Student>(*global);
    EXPECT_TRUE(another != nullptr);

    // How to check they own different object?
    // 要判断两个 shared_ptr 是否指向不同的对象，你可以直接比较这两个智能指针。
    // 在C++中， shared_ptr 的相等性和不等性操作符 ( == 和 !=) 会被重载来比较它们所管理的对象是否是同一个对象。
    EXPECT_TRUE(another != global);
}

// Can the shared_ptr break the Circular Reference?
namespace tr0 {
// std::weak_ptr + forward declaring?
struct A;
struct B;

struct A {
    A(const std::string& str) : name_(str) {}

    void bindB(std::shared_ptr<B> obj) { ptr_ = obj; }

    std::string name_;
    std::shared_ptr<B> ptr_;
};

struct B {
    B(const std::string& str) : name_(str) {}

    void bindA(std::shared_ptr<A> obj) { ptr_ = obj; }

    /*
     * @return
     * nullptr when ptr_ dosen't reference to A
     */
    std::shared_ptr<A> GetA() { return ptr_.lock(); }

    std::string name_;
    std::weak_ptr<A> ptr_;
};

TEST(shared_ptr, circular_reference_breaking) {
    auto a_ptr = std::make_shared<A>("A");
    auto b_ptr = std::make_shared<B>("B");
    a_ptr->bindB(b_ptr);
    b_ptr->bindA(a_ptr);
    EXPECT_EQ("A", a_ptr->name_);
    EXPECT_EQ("B", a_ptr->ptr_->name_);

    EXPECT_EQ("B", b_ptr->name_);
    //  error: base operand of '->' has non-pointer type 'std::weak_ptr<tr0::A>'
    // EXPECT_EQ("A", b_ptr->ptr_->name_);
    EXPECT_TRUE(b_ptr->GetA() != nullptr);
}

TEST(shared_ptr, circular_reference_breaking2) {
    std::shared_ptr<B> b_ptr;

    {
        auto a_ptr = std::make_shared<A>("A");
        b_ptr = std::make_shared<B>("B");
        a_ptr->bindB(b_ptr);
        EXPECT_EQ("A", a_ptr->name_);
        EXPECT_EQ("B", a_ptr->ptr_->name_);

        b_ptr->bindA(a_ptr);
        EXPECT_EQ("B", b_ptr->name_);
        //  error: base operand of '->' has non-pointer type 'std::weak_ptr<tr0::A>'
        // EXPECT_EQ("A", b_ptr->ptr_->name_);
        EXPECT_TRUE(b_ptr->GetA() != nullptr);
    }
    EXPECT_EQ("B", b_ptr->name_);
    EXPECT_FALSE(b_ptr->GetA() != nullptr);
}

}  // namespace tr0
