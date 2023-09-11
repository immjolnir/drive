==========
ROS2
==========


`Why ROS2? <http://design.ros2.org/articles/why_ros2.html>`_
--------------------------------------------------------------

New technologies
''''''''''''''''''''

At the core of ROS is an anonymous publish-subscribe middleware system that is built almost entirely from scratch. Starting in 2007, we built our own systems for discovery, message definition, serialization, and transport. The intervening seven years have seen the development, improvement, and/or widespread adoption of several new technologies that are relevant to ROS in all of those areas, such as:

- Zeroconf;
- Protocol Buffers;
- ZeroMQ (and the other MQs);
- Redis;
- WebSockets; and
- DDS (Data Distribution Service).

It is now possible to build a ROS-like middleware system using off-the-shelf open source libraries. We can benefit tremendously from this approach in many ways, including:

- we maintain less code, especially non-robotics-specific code;
- we can take advantage of features in those libraries that are beyond the scope of what we would build ourselves;
- we can benefit from ongoing improvements that are made by others to those libraries; and
- we can point to existing production systems that already rely on those libraries when people ask us whether ROS is “ready for prime time”.


* rep_0012_

.. _rep_0012: https://ros.org/reps/rep-0012.html
