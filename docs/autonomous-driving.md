# Autonomous Driving 

## 自动驾驶感知职位面试考察哪些知识？
自动驾驶感知职位的面试通常会考察以下知识和技能：

- 计算机视觉基础：包括图像处理、特征提取、图像分类、目标检测、语义分割和实例分割等。

- 深度学习理论与实践：理解并能够应用卷积神经网络（CNN）、循环神经网络（RNN）、Transformer等深度学习模型进行感知任务。

- 目标跟踪技术：熟悉卡尔曼滤波、粒子滤波等传统方法以及现代基于深度学习的跟踪算法。

- 点云处理与激光雷达数据解析：了解如何处理3D点云数据，包括聚类、分割、对象检测和跟踪。

- 多传感器融合：理解如何将来自不同传感器（如摄像头、雷达、激光雷达和IMU）的数据融合以提高感知精度和鲁棒性。

- SLAM（Simultaneous Localization and Mapping）：掌握视觉SLAM、LiDAR SLAM等技术及其在自动驾驶中的应用。

- 数据处理与分析：熟悉大数据处理工具和框架（如Python、Pandas、NumPy等），并能进行数据预处理、标注和后处理。

- 开源框架与工具：熟悉自动驾驶相关的开源库和框架，如TensorFlow、PyTorch、CUDA、ROS（Robot Operating System）等。

- 算法优化与性能评估：理解如何优化算法以减少计算资源消耗和提高实时性，以及如何评估算法的准确率、召回率、F1分数等指标。

- 软件工程实践：良好的编程习惯、代码版本控制（如Git）、软件设计模式和调试技巧。

- 自动驾驶系统架构：理解自动驾驶系统的整体架构，包括感知、定位、规划、控制等模块之间的交互。

- 安全性和可靠性：对自动驾驶的安全关键问题有认识，包括失效模式和效应分析（FMEA）、功能安全（ISO 26262）等。

- 行业标准与法规：了解与自动驾驶相关的主要国际和国内标准、法规以及伦理考量。

在实际面试中，除了理论知识外，面试官可能还会关注候选人的实践经验、项目经历、问题解决能力和团队协作精神。

## 自动驾驶的感知中自车与前车距离怎么计算？
在自动驾驶的感知系统中，计算自车与前车的距离通常采用以下几种方法：

- 毫米波雷达距离估计：毫米波雷达是一种常用的传感器，它可以发射电磁波并接收反射回来的信号，通过测量信号往返的时间来计算距离。这种方法具有较好的精度和可靠性，能够在各种天气条件下工作。

- 激光雷达（LiDAR）测距：激光雷达通过发射激光脉冲并接收反射回来的信号，根据光速和返回时间计算距离。LiDAR可以提供高精度的三维距离信息，有助于构建详细的环境模型。

- 视觉传感器测距：摄像头等视觉传感器可以通过图像处理和计算机视觉技术来估计自车与前车的距离。这通常涉及对前方车辆的检测、跟踪和尺寸估算，然后利用几何关系或深度学习算法计算距离。

- 超声波传感器测距：超声波传感器发射超声波并接收反射回来的信号，根据声波速度和传播时间计算距离。虽然其精度和范围相对较小，但在近距离探测中仍有一定应用。

- 组合多种传感器数据：现代自动驾驶系统通常融合来自不同传感器的数据，以提高距离估计的准确性。例如，通过卡尔曼滤波或其他数据融合算法整合雷达、LiDAR和摄像头的数据，可以减少误差并提高整体性能。

每个自动驾驶系统可能会根据其设计和硬件配置选择最适合的方法或者组合方法来计算自车与前车的距离。这些距离信息对于实现安全的自动驾驶功能至关重要，如自适应巡航控制、紧急制动和路径规划等。

## 障碍物检测的来源深度学习框架有哪些？
深度学习在障碍物检测中广泛应用，以下是一些常用的深度学习框架，这些框架通常包含用于目标检测的库和模型：

- TensorFlow：这是一个开源的机器学习框架，由谷歌开发。它包含Object Detection API，该API支持多种目标检测模型，如Faster R-CNN、Mask R-CNN、SSD和YOLO等。

- PyTorch：这是另一个流行的开源深度学习框架，由Facebook的人工智能研究团队开发。PyTorch提供了torchvision库，其中包含预训练的目标检测模型和数据集，如Faster R-CNN、Mask R-CNN和RetinaNet等。

- Keras：Keras是一个高级的神经网络API，可以运行在TensorFlow、Microsoft Cognitive Toolkit (CNTK) 或 Theano之上。虽然Keras本身不直接提供目标检测的库，但用户可以通过封装底层框架的函数和模型来实现障碍物检测。

- Caffe：Caffe是由伯克利视觉与学习中心（BVLC）开发的一个快速、表达力强的深度学习框架。尽管它的开发活动不如以前活跃，但它仍然被一些研究人员用于目标检测任务。

- MXNet：MXNet是亚马逊主导的开源深度学习框架，它也支持多种目标检测算法的实现。

- Detectron2：这是Facebook AI推出的一个研究平台，基于PyTorch构建，包含了最新的目标检测算法和模型。

- OpenCV：虽然OpenCV主要是一个计算机视觉库，但它也包含了一些深度学习模块，并且可以与上述深度学习框架集成，用于实现障碍物检测。

这些深度学习框架为开发和实施基于深度学习的障碍物检测系统提供了强大的工具和资源。选择哪个框架通常取决于项目的具体需求、开发者对框架的熟悉程度以及社区的支持情况。

## PyTorch的Faster R-CNN原理是什么？
PyTorch中的Faster R-CNN是一种深度学习的目标检测算法，其主要原理包括以下几个部分：

- 卷积神经网络（Convolutional Neural Network, CNN）前处理：
  - 输入图像首先通过一个预训练的卷积神经网络（如VGG、ResNet等），这个阶段的目的是提取图像的特征图（Feature Maps）。
  - 这是网络的初始部分，通常是一个预训练的深度卷积神经网络，如VGG、ResNet等。它的作用是对输入图像进行特征提取，生成一个深层特征图。

- 区域提议网络（Region Proposal Network, RPN）：
  - RPN是一个完全卷积网络，它在特征图上并行地生成多个候选区域（称为anchors）。
  - RPN同时预测每个候选区域的两个输出：一个二维softmax层用于分类（目标或背景），一个线性回归层用于调整候选区域的边界框坐标（bounding box regression）。
  - 候选区域经过非最大抑制（Non-Maximum Suppression, NMS）来去除冗余和重叠的提议。
  - RPN在一个滑动窗口的方式上运行在特征图上，每个位置都会生成多个不同尺度和比例的候选区域（anchors）。
  - RPN有两个分支：一个用于分类，判断候选区域是否包含物体（前景）或背景；另一个用于回归，微调候选区域的边界框位置以更准确地包围物体。
  - RPN通过一个全卷积网络（Fully Convolutional Network, FCN）实现，并输出一系列可能包含物体的区域提议。

- 提案区域选择和ROI Pooling：
  - 根据RPN的输出，选择一定数量的高质量提议区域（例如，具有高目标得分的区域）。
  - 对于每个选定的提议区域，使用ROI Pooling（或RoIAlign在某些实现中）操作将其特征映射到固定尺寸的特征向量，这使得不同大小的提议区域可以被统一处理。
  - 区域of兴趣池化（RoI Pooling / RoIAlign）：
  - 对RPN生成的区域提议，使用RoI池化或RoIAlign层将每个提议区域从特征图上对应的部分提取出固定尺寸的特征向量。
  - 这个过程确保了后续全连接层接收的输入具有统一的尺寸，不论原始提议区域的大小和形状如何。

- Fast R-CNN阶段：
  - 使用ROI Pooling得到的特征向量输入到一个全连接层进行分类和边框回归。
  - 分类层用于判断提议区域是否包含目标以及目标的具体类别。
  - 回归层进一步细化提议区域的边界框坐标，使其更准确地匹配目标对象。
  - 分类和边界框回归（Classification and Bounding Box Regression）：
  - 提取的特征向量经过几个全连接层，分别用于分类（确定提议区域中物体的具体类别）和边界框回归（进一步细化提议区域的位置和大小）。
  - 分类分支预测每个提议区域属于各个预定义类别之一的概率。
  - 回归分支提供一组偏移量，用于调整提议区域的边界框坐标，使其更好地匹配物体的实际位置。

- 后处理和NMS：
  - 对于每个类别的所有提议区域，根据他们的分类得分和回归后的边界框进行排序。
  - 应用非最大抑制算法去除冗余的检测结果，最终得到每张图像中每个类别的最可信目标检测。

Faster R-CNN的主要优势在于它将提议生成和目标检测这两个步骤整合到了一个统一的框架中，并且通过共享卷积特征计算实现了高效的推理速度。
在PyTorch中，这些原理通常通过定义和训练特定的神经网络模块和层来实现。

总的来说，Faster R-CNN的网络结构设计巧妙地结合了特征提取、候选区域生成、分类和定位任务，实现了端到端的训练和高效的目标检测。
这种结构允许网络在单次前向传播中同时完成区域提议和物体分类与定位，从而提高了检测速度和准确性。

## 如何理解他的网络结构？

由于这里是一个文本环境，无法直接提供完整的代码实现。但是，以下是一个简化的Faster R-CNN在PyTorch中的实现步骤概述：
```python 
# 导入必要的库
import torch 
import torchvision from torchvision.models.detection.faster_rcnn 
import FastRCNNPredictor from torchvision.models.detection.rpn 
import AnchorGenerator

# 加载预训练的卷积神经网络作为Backbone： 
python backbone = torchvision.models.resnet50(pretrained=True)

# 定义Anchor生成器和RPN头部 
anchor_generator = AnchorGenerator(sizes=((32, 64, 128, 256, 512),), aspect_ratios=((0.5, 1.0, 2.0),)) 
rpn_head = torchvision.models.detection.rpn.RPNHead(anchor_generator.num_anchors_per_location()[0], backbone.out_channels)

# 创建Faster R-CNN模型： python 
model = torchvision.models.detection.FasterRCNN(backbone, rpn_head=rpn_head, box_roi_pool=None, box_head=None, box_predictor=None)

# 定义分类和回归预测器： python 
num_classes = 2 

# 假设有两类：背景和物体 
in_features = model.roi_heads.box_predictor.cls_score.in_features 
box_predictor = FastRCNNPredictor(in_features, num_classes) 
model.roi_heads.box_predictor = box_predictor
```
训练和推理： 这部分包括数据加载、损失函数定义、优化器设置、训练循环和模型推理等步骤，具体实现会根据你的数据集和需求进行调整。
请注意，这只是一个简化的示例，实际的Faster R-CNN实现可能需要更多的配置和优化。你可以参考PyTorch官方文档或相关的开源项目来获取更详细的代码实现和教程。

## VGG的原理是什么？怎么理解它的网络结构？
VGG网络（Visual Geometry Group Network）是由牛津大学的视觉几何组在2014年提出的深度卷积神经网络模型。其主要原理和网络结构特点如下：

原理：
- 深度可分离性（Depth Separability）：
VGG网络的核心理念是深度可分离性，即认为在网络中增加深度（更多的层次）比增加每个层的复杂性（例如更大的滤波器）更能提高模型的性能。通过使用小尺寸的卷积核（通常是3x3）堆叠多层，VGG网络能够学习更复杂的图像特征。

- 网络结构：
  - 模块化设计：VGG网络采用模块化的设计，其中包含若干个相同的卷积块（Conv Block），每个块由一个或多个连续的卷积层和一个最大池化层组成。这种设计使得网络结构清晰且易于理解和复制。
  - 小卷积核：VGG网络中的卷积层使用3x3的小卷积核，这样的设计可以减少参数数量，同时通过堆叠多层来提取深层次的特征。
  - 步长为1，填充为1：在大多数情况下，VGG网络中的卷积层采用步长为1和填充为1的设置，这使得输入和输出的空间维度保持一致，从而可以堆叠更多的卷积层。
  - 池化层：在网络的某些位置，VGG网络使用最大池化层进行下采样，以减小后续层的计算量并引入空间不变性。
  - 全连接层：在卷积层之后，VGG网络通常会接上一些全连接层（Fully Connected Layers）。这些层将卷积层提取的特征映射转换为固定长度的向量，用于分类任务。
  - 预训练权重：VGG网络的作者公开了预先在ImageNet数据集上训练的权重（如VGG16和VGG19），这些预训练权重可以用于其他图像识别、物体检测或语义分割等任务，作为特征提取的基础。

理解VGG网络的结构主要是把握其逐层堆叠小卷积核和池化层的过程，以及如何通过这种方式逐步提取图像的高级抽象特征。

最终，这些特征经过全连接层转化为类别概率分布，用于图像分类等任务。

在实际应用中，VGG网络的全连接层部分可能被修改或替换为更适合特定任务的结构，如在语义分割中可能被替换为卷积层来生成像素级别的预测。


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