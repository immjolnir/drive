
- pointcloud_preprocess: 点云预处理（过滤调无效点，即过滤坐标为nan的点，扫到自己的部分去掉)
  - 过滤高度超过 z-threhold的点，比如过滤高度大于5米的点
  - 通过自车的点，考虑到激光雷达安装位置，有可能扫到自己，因此过滤自身的点，避免FP

- pointcloud_map_based_roi: 地图ROI过滤
  - ROI 指定可行使区域

- pointcloud_ground_detection: 地面检测
  - 查找地面，为了后续做点云分割

- lidar_detection
  - center_point_detection 是一种基于点云的 anchor-free 的三维目标检测算法
  - cnn_segmentation 自研模型
  - mask_pillars_detection, 基于  point_pillars 做了一些改进
  - point_pillars_detection 通过将点云数据转化为柱形稀疏表示

- lidar_detection_filter

- lidar_tracking

- msg_adapter: 消息转发发布
  - 大对象，直接传递对象的方式，没有通过 topic 的方式发布. 会有 inner 的标志。
  - 把内部消息 "inner" 转化成外读的，可以通过cyber_monitor 查看.


障碍物信息: 通过点云检测到的障碍物信息，包括目标的类别，大小，速度以及track id等，用于后续的规划模块进行路径规划，发布消息的通道为 /apollo/perception/obstacles

坐标转换
/tf
/tf/static

- lidar_detection
  |_ conf // 组件配置文件
  |_ dag
  |_ data // 功能配置文件, 算法的配置
  |_ detector
       |_ center_point_detection
  |_ interface
  |_ proto
  |_ lidar_detection_component.cc
  |_ lidar_detection_component.h
  |_ cyb

公共配置
modules/percepton/data
  |_ BUILD
  |_ conf // 感知公共配置，例如一些单例
  |_ flags // 感知模块所有的gflags命令行传入，用于dag
  |_ models // 模型的配置
