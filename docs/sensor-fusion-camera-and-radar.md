# 1. 相机与毫米波雷达融合的目标检测方法前沿进展

- https://imagine.gsfc.nasa.gov/science/toolbox/emspectrum1.html
- https://github.com/nacayu/CRFNet_Tensorflow2.4.1
- https://github.com/ZHOUYI1023/awesome-radar-perception
- TI 毫米波基础知识手册
  - https::/www.ti.com/cn/lit/wp/zhcy075/zhcy075.pdf

- Torwads Deep Radar Perception for Autonomouse Driving: DataSets, Methods, and Challenge
- MmWave Raar and Vision Fusion for Object Detection in Autonomous Driving: A Review
- Multi-model Sensor Fusion for Auto Driving Perception: A Survey
- Comparative Analysis of Radar and Lidar Technologies for Automotive Applications
- Deep Learning-based Radar, Camera and Lidar Fusion for Object Detection.


- Early Fusion
  - RRPN, RadSegNet
  - 最小化数据损失，融合简便
  - 实时性，受标定效果影响大，检测结果受radar点云数量影响大

- Feature Fusion
  - CRFNet
  - CenterFusion
  - FUTR3D
  - 充分利用传感器信息达到模态互补作用
  - 传感器信噪比不一，对传感器的联合概率密度函数进行建模较为困难

- Late Fusion
  - YOdar: Uncertainty-based Sensor Fusion for Vehicle Detection with Camera and Radar Sensors
  - 各模态分支拥有更大灵活性
  - 不能有效发挥模态之间的互补性

- Asymmetry Fusion
  - CRAFT
  - RCBEV
  - 能够利用多个阶段融合的优势
  - realtime，多阶段组合设计较为复杂

Timeline
```
                                         .-----------------------------------------.
                                         |               Asymmetry Fusion          |
                                         |   GRIF NET(2020/10)    CRAFT(2022/09)   |
                                         '-----------------------------------------'
           .-----------------------------------------.
           |           Feature Fusion                |
           | CRFNet(2020/01)  CenterFusion(2020/11)  |
           '-----------------------------------------'
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
.--------------------------------.
|       Early Fusion             |
|RPRN(2019/05) SAF-FCOS(2020/11) |
'--------------------------------'
```

## Go Deeper with their source code
- CRFNET(2020/01)
- CenterFusion(2020/11)
  - 承上启下CRFNet: 在滤波、表征能力、引入先验上都做出了改进
  - ROI Frustum 将点云空间滤波与特征提取解耦，能够实现端到端的点云空间滤波和特征关联
  - CRFNet 和 CenterFusion 都致力于将点云进行高度或者宽度上的扩充，以引入先验扩增点云的覆盖范围

- CRAFT(2022/09)
  - 消融实验
  - 径向方差

## 数据集
- RadarScenes

- nuScenes
  - 但对radar 没有label

传统3D毫米波雷达
- 无高度信息
- 多jing干扰

4D Radar

Radard的数据增强方法
极端天气


## Details
- 聚类算法
  - k-means
  - DBSCAN

- 匹配算法
  - 匈牙利匹配算法

- 滤波算法
  - KF
  - EKF

- 基于 EKF/UKF 的雷达后融合感知算法

- 基于深度学习的点云检测任务
  - RODNet
  - Radar-PointGNN
  - Radar-PointNets

- 毫米波雷达视觉融合 2D 检测任务
  - CRFNet
  - RPRN
  - SAF-FCOS

- 毫米波雷达视觉融合 3D 检测任务
  - CenterFusion
  - RCBEV
  - CRAFT
  - FUTR3D


# 2. 后融合感知

- 量产中的多传感器后融合方案
  - Object Level Fusion for Surround Environment Perception in Automated Driving Applications(2017)

- 状态估计和几何变换
  - 《机器人学中的状态估计》, State Estimation for Robotics, Timothy Barfoot
  - 《概率机器人》，Probabilistic Robotics, Sebastain Thrun, Wolfram Burgard, Dieter Fox

- 检测
```

Camera Image     --> Camera Objects ---.---------.
                                       | Fusion  |
Lidar Point Cloud --> Lidar Objects ---|  &      |
                                       | Tracking|
Radar Point Cloud --> Radar Objects ---|         |
                                       '---------'
```
- 目标跟踪
  - 每个目标的唯一编号
  - 每个目标的动态信息(速度，加速度，角速度）Motion
  - 多帧跟踪来减小估计误差(位置，朝向，类别，尺寸等）
  - 保持对局部遮挡目标的感知结果
  - 状态估计
  - 数据关联
  - 轨迹管理






































