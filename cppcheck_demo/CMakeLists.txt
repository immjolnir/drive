cmake_minimum_required(VERSION 3.10)

project(CppCheckDemoProject)

file(
    WRITE "main.cpp"
[=[
int main()
{
    char a[10];
    a[10] = 0;
    return 0;
}
]=]
)

set(CMAKE_CXX_CPPCHECK "cppcheck")
add_executable(${PROJECT_NAME} "main.cpp")
