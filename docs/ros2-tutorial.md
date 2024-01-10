# [ROS2 Tutorial](https://docs.ros.org/en/iron/Tutorials/Beginner-CLI-Tools/Configuring-ROS2-Environment.html)

## Background
ROS 2 relies on the notion of combining workspaces using the shell environment.
“Workspace” is a ROS term for the location on your system where you’re developing with ROS 2.
The core ROS 2 workspace is called the __underlay__. 
Subsequent local workspaces are called __overlays__.
When developing with ROS 2, you will typically have several workspaces active concurrently.

Combining workspaces makes developing against different versions of ROS 2, or against different sets of packages, easier. It also allows the installation of several ROS 2 distributions (or “distros”, e.g. Dashing and Eloquent) on the same computer and switching between them.

This is accomplished by sourcing setup files every time you open a new shell, or by adding the source command to your shell startup script once. Without sourcing the setup files, you won’t be able to access ROS 2 commands, or find or use ROS 2 packages. In other words, you won’t be able to use ROS 2.

- Source the setup files
```
# Replace ".bash" with your shell if you're not using bash
# Possible values are: setup.bash, setup.sh, setup.zsh
source /opt/ros/iron/setup.bash
```

- Add sourcing to your shell startup script
```
echo "source /opt/ros/iron/setup.bash" >> ~/.bashrc
```

- Check environment variables
```
printenv | grep -i ROS
```
Check that variables like ROS_DISTRO and ROS_VERSION are set.

```
ROS_VERSION=2
ROS_PYTHON_VERSION=3
ROS_DISTRO=iron
```
If the environment variables are not set correctly, return to the ROS 2 package installation section of the installation guide you followed. 


## The ROS_DOMAIN_ID variable
- [ROS_DOMAIN_ID](https://docs.ros.org/en/iron/Concepts/Intermediate/About-Domain-ID.html)

```
export ROS_DOMAIN_ID=<your_domain_id>
```

## The ROS_AUTOMATIC_DISCOVERY_RANGE variable
By default, ROS 2 communication is not limited to localhost.
__ROS_AUTOMATIC_DISCOVERY_RANGE__ environment variable allows you to limit ROS 2 discovery range.
Using __ROS_AUTOMATIC_DISCOVERY_RANGE__ is helpful in certain settings, such as classrooms, where multiple robots may publish to the same topic causing strange behaviors.
See [Improved Dynamic Discovery](https://docs.ros.org/en/iron/Tutorials/Advanced/Improved-Dynamic-Discovery.html#improveddynamicdiscovery) for more details.

