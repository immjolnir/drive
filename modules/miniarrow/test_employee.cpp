#include <gtest/gtest.h>

using namespace std;

#include <iostream>
#include <string>

class Employee {
  public:
    Employee(unsigned int id, const std::string& firstName, const std::string& lastName);
    unsigned int getId() const;
    const std::string& getFirstName() const;
    const std::string& getLastName() const;
    friend std::ostream& operator<<(std::ostream& os, const Employee& emp);

  private:
    unsigned int id;
    std::string firstName;
    std::string lastName;
};

Employee::Employee(unsigned int id, const std::string& firstName, const std::string& lastName)
  : id{id}, firstName{firstName}, lastName{lastName} {
    if (id == 0) {
        throw invalid_argument("id must be greater than zero.");
    }
}

unsigned int Employee::getId() const { return id; }

const std::string& Employee::getFirstName() const { return firstName; }

const std::string& Employee::getLastName() const { return lastName; }

std::ostream& operator<<(std::ostream& os, const Employee& emp) {
    os << emp.firstName << " " << emp.lastName;
    return os;
}

TEST(Employee_Constructor, AllValidArgs_ReturnSuccess) {
    Employee sample(1, "Joe", "Blow");
    EXPECT_EQ(1, sample.getId());
    EXPECT_EQ("Joe", sample.getFirstName());
    EXPECT_EQ("Blow", sample.getLastName());
}

TEST(Employee_Constructor, IdIsZero_ThrowInvalidArgument) {
    try {
        Employee sample(0, "Joe", "Blow");
        FAIL();
    } catch (invalid_argument& err) {
        EXPECT_STREQ("id must be greater than zero.", err.what());
    }
}
