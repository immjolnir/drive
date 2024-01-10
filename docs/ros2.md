# [ROS 2 Documentation](https://docs.ros.org/en/iron/index.html)
The Robot Operating System (ROS) is a set of software libraries and tools for building robot applications. From drivers and state-of-the-art algorithms to powerful developer tools, ROS has the open source tools you need for your next robotics project.

# [Iron Releases](https://docs.ros.org/en/iron/Releases.html)

- What is a Distribution?
A ROS distribution is a versioned set of ROS packages. These are akin to Linux distributions (e.g. Ubuntu). The purpose of the ROS distributions is to let developers work against a relatively stable codebase until they are ready to roll everything forward. Therefore once a distribution is released, we try to limit changes to bug fixes and non-breaking improvements for the core packages (every thing under ros-desktop-full). That generally applies to the whole community, but for “higher” level packages, the rules are less strict, and so it falls to the maintainers of a given package to avoid breaking changes.

- Below is a list of current and historic ROS 2 distributions. 
  - Iron Irwini
  - Humble Hawksbill
  - Galactic Geochelone
  - Foxy Fitzroy
  - Eloquent Elusor
  - Dashing Diademata

## [Running ROS 2 nodes in Docker [community-contributed]](https://docs.ros.org/en/iron/How-To-Guides/Run-2-nodes-in-single-or-separate-docker-containers.html)

### Run two nodes in a single docker container
- Pull the ROS docker image with tag “iron-desktop”.
```
$ docker pull osrf/ros:iron-desktop
```

- Your best friend is the ros2 command line help now.
```
root@<container-id>:/# ros2 --help
```

- E.g. list all installed packages.

root@<container-id>:/# ros2 pkg list
(you will see a list of packages)


- E.g. list all executables:

root@<container-id>:/# ros2 pkg executables
(you will see a list of <package> <executable>)

- Run a minimal example of 2 C++ nodes (1 topic subscriber listener, 1 topic publisher talker) from the package demo_nodes_cpp in this container:

ros2 run demo_nodes_cpp listener &
ros2 run demo_nodes_cpp talker

### Run two nodes in two separate docker containers
- Open a terminal. 
Run the image in a container in interactive mode and launch a topic publisher (executable talker from the package demo_nodes_cpp) with ros2 run:

docker run -it --rm osrf/ros:iron-desktop ros2 run demo_nodes_cpp talker

- Open a second terminal. Run the image in a container in interactive mode and launch a topic subscriber (executable listener from the package demo_nodes_cpp) with ros2 run:

docker run -it --rm osrf/ros:iron-desktop ros2 run demo_nodes_cpp listener

- As an alternative to the command line invocation, you can create a docker-compose.yml file (here version 2) with the following (minimal) content:
```yaml
version: '2'

services:
  talker:
    image: osrf/ros:iron-desktop
    command: ros2 run demo_nodes_cpp talker
  listener:
    image: osrf/ros:iron-desktop
    command: ros2 run demo_nodes_cpp listener
    depends_on:
      - talker
```
To run the containers call docker compose up in the same directory. You can close the containers with `Ctrl+C`.


# [ROS2: Installing from source](https://design.ros2.org/articles/cc_build_tools.html)

## [Building from source](https://docs.ros.org/en/iron/Installation/Alternatives/Ubuntu-Development-Setup.html)
We support building ROS 2 from source on the following platforms:


## [Source the environment](https://docs.ros.org/en/iron/Tutorials/Beginner-Client-Libraries/Colcon-Tutorial.html#source-the-environment)

When colcon has completed building successfully, the output will be in the __install__ directory. 
Before you can use any of the installed executables or libraries, you will need to add them to your path and library paths. 
colcon will have generated bash/bat files in the __install__ directory to help set up the environment.
These files will add all of the required elements to your path and library paths as well as provide any bash or shell commands exported by packages.

## Try a demo
With the environment sourced, we can run executables built by colcon. Let’s run a subscriber node from the examples:
```
ros2 run examples_rclcpp_minimal_subscriber subscriber_member_function
```

In another terminal, let’s run a publisher node (don’t forget to source the setup script):
```
ros2 run examples_rclcpp_minimal_publisher publisher_member_function
```

You should see messages from the publisher and subscriber with numbers incrementing.


## Create your own package
- https://github.com/ros2/demos/tree/iron/demo_nodes_cpp

A package such as demo_nodes_cpp uses the `ament_cmake` build type, and uses CMake as the build tool.

For convenience, you can use the tool `ros2 pkg create` to create a new package based on a template.


# [Testing](https://docs.ros.org/en/iron/Tutorials/Intermediate/Testing/Testing-Main.html)
## Why automatic tests?

Here are some of the many good reasons why should we have automated tests:

- You can make incremental updates to your code more quickly. ROS has hundreds of packages with many interdependencies, so it can be hard to anticipate the problems a small change might cause. If your change passes the unit tests, you can be more confident that you haven’t introduced problems — or at least the problems aren’t your fault.

- You can refactor your code with greater confidence. Passing the unit tests verifies that you haven’t introduced any bugs while refactoring. This gives you this wonderful freedom from change fear!

- It leads to better designed code. Unit tests force you to write your code so that it can be more easily tested. This often means keeping your underlying functions and framework separate, which is one of our design goals with ROS code.

- They prevent recurring bugs (bug regressions). It’s a good practice to write a unit test for every bug you fix. In fact, write the unit test before you fix the bug. This will help you to precisely, or even deterministically, reproduce the bug, and much more precisely understand what the problem is. As a result, you will also create a better patch, which you can then test with your regression test to verify that the bug is fixed. That way the bug won’t accidentally get reintroduced if the code gets modified later on. It also means that it will be easier to convince the reviewer of the patch that the problem is solved, and the contribution is of high quality.

- Other people can work on your code more easily (an automatic form of documentation). It can be hard to figure out whether or not you’ve broken someone else’s code when you make a change. The unit tests are a tool for other developers to validate their changes. Automatic tests document your coding decisions, and communicate to other developers automatically about their violation. Thus tests become documentation for your code — a documentation that does not need to be read for the most time, and when it does need to be inspected the test system will precisely indicate what to read (which tests fail). By writing automatic tests you make other contributors faster. This improves the entire ROS project.

- It is much easier to become a contributor to ROS if we have automated unit tests. It is very difficult for new external developers to contribute to your components. When they make changes to code, they are often doing it in the blind, driven by a lot of guesswork. By providing a harness of automated tests, you help them in the task. They get immediate feedback for their changes. It becomes easier to contribute to a project, and new contributors to join more easily. Also their first contributions are of higher quality, which decreases the workload on maintainers. A win-win!

- Automatic tests simplify maintainership. Especially for mature packages, which change more slowly, and mostly need to be updated to new dependencies, an automatic test suite helps to very quickly establish whether the package still works. This makes it much easier to decide whether the package is still supported or not.

- Automatic tests amplify the value of Continuous Integration. Regression tests, along with normal scenario-based requirements tests, contribute to overall body of automated tests for your component. Your component is better tested against evolution of other APIs that it depends on (CI servers will tell you better and more precisely what problems develop in your code).

- Perhaps the most important benefit of writing tests is that tests make you a good citizen. Tests influence quality in the long term. It is a well accepted practice in many open-source projects. By writing regressions tests, you are contributing to long term quality of the ROS ecosystem.

## Is this all coming for free?
Of course, there is never free lunch. To get the benefits of testing, some investment is necessary.

- You need to develop a test, which sometimes may be difficult or costly. Sometimes it might also be nontrivial, as the test should be automatic. Things get particularly hairy if your tests should involve special hardware (they should not: try to use simulation, mock the hardware, or narrow down the test to a smaller software problem) or require external environment, for instance human operators.

- Regression tests and other automatic tests need to be maintained. When the design of the component changes, a lot of tests become invalidated (for instance they no longer compile, or throw runtime exceptions related to the API design). These tests fail not only because the redesign re-introduced bugs but also because they need to be updated to the new design. Occasionally, with bigger redesigns, old regression tests should be dropped.

Large bodies of tests can take a long time to run, which can increase Continuous Integration server costs.

## [Writing Basic Tests with C++ with GTest](https://docs.ros.org/en/iron/Tutorials/Intermediate/Testing/Cpp.html)

- Source Code
We’ll start off with our code in a file called test/tutorial_test.cpp
```c++
#include <gtest/gtest.h>

TEST(package_name, a_first_test)
{
  ASSERT_EQ(4, 2 + 2);
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
```

- package.xml
Add the following line to package.xml
```
<test_depend>ament_cmake_gtest</test_depend>
```

- CMakeLists.txt
```
if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(${PROJECT_NAME}_tutorial_test test/tutorial_test.cpp)
  target_include_directories(${PROJECT_NAME}_tutorial_test PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  )
  ament_target_dependencies(${PROJECT_NAME}_tutorial_test
    std_msgs
  )
  target_link_libraries(${PROJECT_NAME}_tutorial_test name_of_local_library)
endif()
```

## [Running Tests in ROS 2 from the Command Line](https://docs.ros.org/en/iron/Tutorials/Intermediate/Testing/CLI.html)
- Build and run your tests
To compile and run the tests, simply run the test verb from colcon.
```
colcon test --ctest-args tests [package_selection_args]
```
(where package_selection_args are optional package selection arguments for colcon to limit which packages are built and run)

Sourcing the workspace before testing should not be necessary. colcon test makes sure that the tests run with the right environment, have access to their dependencies, etc.

- Examine Test Results
To see the results, simply run the test-result verb from colcon.
```
colcon test-result --all
```

To see the exact test cases which fail, use the `--verbose` flag:
```
colcon test-result --all --verbose
```

- Debugging tests with GDB
If a C++ test is failing, gdb can be used directly on the test executable in the build directory. Ensure to build the code in debug mode. Since the previous build type may be cached by CMake, clean the cache and rebuild.
```
colcon build --cmake-clean-cache --mixin debug
```

Next, run the test directly through gdb. For example:
```
gdb -ex run ./build/rcl/test/test_logging
```

## [Write Basic test with python ](https://docs.ros.org/en/iron/Tutorials/Intermediate/Testing/Python.html)



## Links
- https://zhuanlan.zhihu.com/p/524589665
- https://github.com/ros2/ros2/blob/rolling/ros2.repos
- https://github.com/osrf/docker_images/blob/master/ros2/source/source/Dockerfile
