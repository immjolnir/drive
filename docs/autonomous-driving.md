# Autonomous Driving 

## Planning

在规划中，结合高精地图，进行定位和预测来构建车辆轨迹。规划过程主要分为两步：
- 路线规划，地图路线导航，从A到B

- 轨迹规划，目标是生成免碰撞和舒适的可执行轨迹，该轨迹由一系列点定义，每个点都由一个关联速度和一个指示何时应抵达那个点的时间戳。

Input: 
- Current Position: A on Map
- Target Position: B on Map

接下来将输入的地图数据转换为 图 Graph, 利用 A* 算法生成轨迹。
轨迹生成的目标是生成由一系列路径点定义的轨迹，为每个路径点分配一个时间戳和速度，我们让一条曲线于这些路径点拟合，生成轨迹的几何表征，然后多条轨迹选择最佳的一条。

由于移动的障碍物可能会暂时阻挡部分路段，轨迹中的每个路点都有时间戳，因此可以将时间戳与预测模块的输出相结合，以确保在计划通过时，轨迹上的每个路径点均未被占用。
这些时间戳创建了一个三维轨迹，每个路径点由空间中的两个维度以及时间上的第三个维度来定义。
我们还为每个路径点指定一个速度，速度用于确保车辆按时到达每个路径点，并且设置了多种约束条件，例如轨迹应能免于碰撞，要让乘客感到舒适，路径点之间的过渡以及速度的任何变化必须平滑，
可行的实际路径的验证和对现实交通规则匹配。

### Frenet坐标系

## Further reading
- Control theory: https://en.wikipedia.org/wiki/Control_theory#Main_control_strategies
- Self-Tuning PID Controller for Autonomous Car Tracking in Urban Traffic: http://oa.upm.es/30015/1/INVE_MEM_2013_165545.pdf
- The Twiddle algorithm for tuning PID controllers: https://martin-thoma.com/twiddle/
- Lateral Tracking Control for the Intelligent Vehicle Based on Adaptive PID Neural Network: https://www.ncbi.nlm.nih.gov/pmc/articles/PMC5492364/
- MPC-Based Approach to Active Steering for Autonomous Vehicle Systems: https://borrelli.me.berkeley.edu/pdfpub/pub-6.pdf
- Kinematic and Dynamic Vehicle Models for Autonomous Driving Control Design: https://borrelli.me.berkeley.edu/pdfpub/IV_KinematicMPC_jason.pdf

## Reference:
- [Baidu Apollo](https://github.com/ApolloAuto/apollo)

- [Hands-On Vision and Behavior for Self-Driving Cars](https://github.com/PacktPublishing/Hands-On-Vision-and-Behavior-for-Self-Driving-Cars)
    - [Code In Action on Youtube](https://www.youtube.com/playlist?list=PLeLcvrwLe187YY2FGalmfzRsjk-GUJ_sA)

- [Learning OpenCV 4 Computer Vision with Python 3 - Third Edition](https://github.com/PacktPublishing/Learning-OpenCV-4-Computer-Vision-with-Python-Third-Edition)

- [Hands-On GPU Accelerated Computer Vision with OpenCV and CUDA](https://github.com/PacktPublishing/Hands-On-GPU-Accelerated-Computer-Vision-with-OpenCV-and-CUDA)


## Visualizing Tools
- cloudcompare
  - vis lidar/images