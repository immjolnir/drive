# ROS1




# [ROS1: Installing from source](https://wiki.ros.org/Installation/Source)

# Melodic
Install from source requires that you download and compile the source code on your own. ROS **Melodic** supports Ubuntu Artful and Bionic as well as Debian Stretch.

## Prerequisites

- Install bootstrap dependencies (Ubuntu):
These tools are used to facilitate the download and management of ROS packages and their dependencies, among other things.

```
$ sudo apt-get install python-rosdep python-rosinstall-generator python-vcstool python-rosinstall build-essential
```

- Initializing rosdep
```
$ sudo rosdep init
$ rosdep update
```

## Installation
Start by building the core ROS packages.

- Create a catkin Workspace
In order to build the core packages, you will need a catkin workspace. Create one now:
```
$ mkdir ~/ros_catkin_ws
$ cd ~/ros_catkin_ws
```

Next we will want to fetch the core packages so we can build them. We will use vcstool for this. Select the vcstool command for the particular variant you want to install:
- Desktop-Full Install: ROS, rqt, rviz, robot-generic libraries, 2D/3D simulators, navigation and 2D/3D perception
```
$ rosinstall_generator desktop_full --rosdistro melodic --deps --tar > melodic-desktop-full.rosinstall
$ vcs import src < melodic-desktop-full.rosinstall
```

- Desktop Install (recommended): ROS, rqt, rviz, and robot-generic libraries
```
$ rosinstall_generator desktop --rosdistro melodic --deps --tar > melodic-desktop.rosinstall
$ mkdir src
$ vcs import src < melodic-desktop.rosinstall
```

- ROS-Comm: (Bare Bones) ROS package, build, and communication libraries. No GUI tools.
```
$ rosinstall_generator ros_comm --rosdistro melodic --deps --tar > melodic-ros_comm.rosinstall
$ vcs import src < melodic-ros_comm.rosinstall
```
This will add all of the catkin packages in the given variant and then fetch the sources into the `~/ros_catkin_ws/src` directory.
The command will take a few minutes to download all of the core ROS packages into the src folder. 
The `-j8` option downloads 8 packages in parallel.

## Resolving Dependencies
Before you can build your catkin workspace you need to make sure that you have all the required dependencies. We use the rosdep tool for this:

```
$ rosdep install --from-paths src --ignore-src --rosdistro melodic -y
```

This will look at all of the packages in the src directory and find all of the dependencies they have. Then it will recursively install the dependencies.

The `--from-paths` option indicates we want to install the dependencies for an entire directory of packages, in this case src. 
The `--ignore-src` option indicates to rosdep that it shouldn't try to install any ROS packages in the src folder from the package manager, we don't need it to since we are building them ourselves. 
The `--rosdistro` option is required because we don't have a ROS environment setup yet, so we have to indicate to rosdep what version of ROS we are building for. 
Finally, the `-y` option indicates to rosdep that we don't want to be bothered by too many prompts from the package manager.

After a while (and maybe some prompts for your password) rosdep will finish installing system dependencies and you can continue.

## Building the catkin Workspace
Once it has completed downloading the packages and resolving the dependencies you are ready to build the catkin packages.
We will use the `catkin_make_isolated` command because there are both catkin and plain cmake packages in the base install, when developing on your catkin only workspaces you may choose to use catkin/commands/catkin_make which only works with catkin packages.

Invoke catkin_make_isolated:
```
$ ./src/catkin/bin/catkin_make_isolated --install -DCMAKE_BUILD_TYPE=Release
```
> Note: You might want to select a different CMake build type (e.g. RelWithDebInfo or Debug)

> Note: The default catkin installation location would be ~/ros_catkin_ws/install_isolated, if you would like to install somewhere else then you can do this by adding the --install-space /opt/ros/melodic argument to your catkin_make_isolated call.

> For usage on a robot without Ubuntu, it is recommended to install compiled code into /opt/ros/melodic just as the Ubuntu packages would do. Don't do this in Ubuntu, as the packages would collide with apt-get packages. It is also possible to install elsewhere (e.g. /usr), but it is not recommended unless you really know what you are doing.

Now the packages should have been installed to ~/ros_catkin_ws/install_isolated or to wherever you specified with the --install-space argument. If you look in that directory you will see that a setup.bash file have been generated. To utilize the things installed there simply source that file like so:
```
$ source ~/ros_catkin_ws/install_isolated/setup.bash
```

# Reference

- If you rely on these packages, please support OSRF.
  - https://github.com/osrf/docker_images

- https://github.com/ika-rwth-aachen/docker-ros

- ROS packages hold at here
  - https://github.com/ros-gbp/ros-release/tags



## https://www.ros.org/reps/rep-0150.html
End-user entry points
We define the same three main entry points for ROS users as REP 108 [1].

desktop_full (recommended)
desktop
ros_base
Metapackages
ROS Core
The ros_core metapackage composes the core communication protocols. It may not contain any GUI dependencies.

- ros_core:
     packages: [catkin, class_loader, cmake_modules, common_msgs, gencpp,
                geneus, genlisp, genmsg, gennodejs, genpy, message_generation,
                message_runtime, pluginlib, ros, ros_comm,
                rosbag_migration_rule, rosconsole, rosconsole_bridge,
                roscpp_core, rosgraph_msgs, roslisp, rospack, std_msgs,
                std_srvs]
ROS Base
The ros_base metapackage composes the ros_core metapackage with commonly used libraries. It may not contain any GUI dependencies.

- ros_base:
    extends: [ros_core]
    packages: [actionlib, bond_core, dynamic_reconfigure, nodelet_core]
Robot metapackage
The robot metapackage is defined to be core, stable, ROS libraries for any robot hardware. It is the "general robotics" libraries of ROS. It may not contain any GUI dependencies.

- robot:
    extends: [ros_base]
    packages: [control_msgs, diagnostics, executive_smach, filters, geometry,
               joint_state_publisher, kdl_parser, kdl_parser_py,
               robot_state_publisher, urdf, urdf_parser_plugin, xacro]
Capability variants
The capability variants organize commonly used libraries that are specific to a class of use case. We also define a simulators variant that provides an organizational role for higher-level variants. We discourage GUI dependencies in these stacks, if possible.

- perception:
    extends: [ros_base]
    packages: [image_common, image_pipeline,
               image_transport_plugins, laser_pipeline,
               perception_pcl, vision_opencv]

- simulators:
    extends: [robot]
    packages: [gazebo_ros_pkgs, rqt_common_plugins, rqt_robot_plugins,
               stage_ros]

- viz:
    extends: [ros_base]
    packages: [rqt_common_plugins, rqt_robot_plugins, rviz]
Desktop variants
The desktop variants are main entry points for users. The desktop_full is a "batteries included" experience for users and attempts to collect stable, well-documented libraries. These libraries may be specific to certain classes of robots, such as mobile robots, though they are not specific to a particular robot. The desktop variant is more minimal and only provides the stacks in the robot variant, plus visualization and debugging tools. Both of these variants contain tutorials for the stacks they provide.

- desktop:
    extends: [robot, viz]
    packages: [angles, common_tutorials, geometry_tutorials, joint_state_publisher_gui,
               ros_tutorials, roslint, urdf_tutorial, visualization_tutorials]
- desktop_full:
    extends: [desktop, perception, simulators]
    packages: [urdf_sim_tutorial]

> Note: If you see an error relating to the EMPY module being missing you may be building using Python 2 rather than Python 3. 
> To ensure you are using the latter, append the the following to the command above. If you have Python 3 installed elsewhere, update the path accordingly.
> -DPYTHON_EXECUTABLE=/usr/bin/python3


# Docker command

```
$ docker run -u $(id -u):$(id -g) -it --net=host --ipc=host 
```


