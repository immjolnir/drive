# Object Tracking

- [Visual Object Tracking | Papers With Code](https://paperswithcode.com/task/visual-object-tracking)
  - Visual Object Tracking is an important research topic in computer vision, image understanding and pattern recognition. Given the initial state (centre location and scale) of a target in the first frame of a video sequence, the aim of Visual Object Tracking is to automatically obtain the states of the object in the subsequent video frames. Source: [Learning Adaptive Discriminative Correlation Filters via Temporal Consistency Preserving Spatial Feature Selection for Robust Visual Tracking](https://arxiv.org/abs/1807.11348)

- [Multiple Object Tracking | Papers With Code](https://paperswithcode.com/task/multiple-object-tracking)
  - Multiple Object Tracking is the problem of automatically identifying multiple objects in a video and representing them as a set of trajectories with high accuracy. Source: [SOT for MOT](https://arxiv.org/abs/1712.01059)



## [单目标跟踪（VOT）经典算法简介](https://zhuanlan.zhihu.com/p/599664250)

在CV领域，Visual Object Tracking（VOT）和Multiple Object Tracking（MOT）虽然都属于目标跟踪，通常都和Object Detection一起使用，但其中还是有一些本质上的区别：

- VOT又被称为SOT（Single Object Tracking），其跟踪给定的初始单个目标，然后在新的帧上找到和跟踪目标最匹配的区域（类似于模板匹配），就为目标新的位置；

- MOT则是跟踪多个目标，相比于VOT，除了要找到跟踪的目标外，还要能够区分跟踪目标属于初始目标中的哪一个（相当于VOT的检测之后，做进一步识别）。
