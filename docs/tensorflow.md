# AVX required
- issue： https://github.com/tensorflow/tensorflow/issues/20248



## AVX指令集有哪些？怎么使用？

- CPU with AVX support 是什么意思？

CPU with AVX support是指处理器支持Advanced Vector Extensions (AVX) 指令集。AVX是Intel和AMD等芯片制造商在现代处理器中引入的一种扩展指令集，旨在提高处理器在进行浮点运算、多媒体处理和其他计算密集型任务时的性能和效率。
通过AVX指令集，处理器能够并行处理更多的数据，因为这些指令允许更大规模的矢量操作。这在诸如科学计算、3D图形渲染、视频编码/解码、机器学习和大数据分析等领域特别有用，这些领域常常需要对大量数据进行高速运算。
换句话说，如果一个CPU支持AVX，那么它就能够利用这些高级矢量指令来加速特定类型的计算工作负载，从而提高整体性能。一些较新的软件和应用程序，如MongoDB 5.0+和《帝国时代4》，可能会要求CPU必须支持AVX才能正常运行或达到最佳性能。
如果您当前的CPU不支持AVX，可能需要更新BIOS（在某些情况下可以启用AVX支持）或者考虑更换支持AVX的处理器。


AVX（Advanced Vector Extensions）指令集是Intel和AMD为提升处理器的并行计算能力而引入的一系列扩展指令。以下是一些主要的AVX指令集及其特点：
AVX (Advanced Vector Extensions)：
• 首次引入了256位的向量寄存器（ymm），相比之前的128位SSE指令集，能够处理更多的数据。
• 支持单精度（float，32位）和双精度（double，64位）浮点数运算。
AVX2：
• 在AVX的基础上扩展了更多的指令，包括整数运算指令。
• 提供了更丰富的数据类型支持和更复杂的运算操作，如混合整数和浮点运算、字节和位操作等。
AVX-512：
• 进一步扩展到512位的向量寄存器（zmm）。
• 提供了更多的 SIMD（Single Instruction Multiple Data）指令，以进一步提高并行计算性能。
• 分为多个子集，如AVX-512F（Foundation）、AVX-512CD（Conflict Detection）、AVX-512BW（Byte and Word）、AVX-512DQ（Doubleword and Quadword）等，每个子集针对特定类型的运算进行优化。
使用AVX指令集通常涉及到以下步骤：
检查硬件支持：
• 确保你的处理器支持AVX指令集。这可以通过查看处理器规格或使用CPUID指令来检测。
编译器设置：
• 在编译代码时，需要启用相应的编译器选项来支持AVX指令集。例如，在GCC或Clang中，可以使用 
-mavx• 、
-mavx2•  或 
-mavx512•  等选项。
使用 intrinsic 函数：
• 大多数现代编程语言（如C/C++和汇编）提供了 intrinsic 函数，这些函数映射到特定的AVX指令。通过使用这些函数，可以更容易地编写和理解使用AVX指令的代码。
数据对齐：
• 为了获得最佳性能，通常需要确保操作的数据在内存中是对齐的，以避免因非对齐访问导致的性能下降。
手动优化：
• 根据具体的应用场景和算法，可能需要手动调整和优化代码，以充分利用AVX指令集的特性。
以下是一个简单的AVX intrinsic函数使用示例（C++）：
```c++
#include <immintrin.h>

void addArrays(float* a, float* b, float* c, int size) {
    __m256 va, vb, vc;
    for (int i = 0; i < size; i += 8) {
        va = _mm256_load_ps(a + i);
        vb = _mm256_load_ps(b + i);
        vc = _mm256_add_ps(va, vb);
        _mm256_store_ps(c + i, vc);
    }
}

```
在这个例子中，
_mm256_load_ps、
_mm256_add_ps 和 
_mm256_store_ps 是分别用于加载、加法和存储256位向量的 intrinsic 函数。这个函数实现了两个浮点数数组的元素级加法，并利用AVX指令集加速计算。


## latest-gpu and latest-gpu-jupyter 这两个docker image有哪些不同？
latest-gpu 和 latest-gpu-jupyter 这两个 Docker 镜像的主要区别在于它们预装的软件和目标用途。

- latest-gpu： 这个镜像通常是指一个基础镜像，它包含了支持GPU运算的驱动和库，比如NVIDIA CUDA和cuDNN。这样的镜像主要目的是为开发者或研究人员提供一个能够在GPU环境下运行计算密集型任务的基础环境，如深度学习、科学计算等。用户可以根据自己的需求在这个基础上安装和配置所需的开发环境和工具。

- latest-gpu-jupyter： 这个镜像在 latest-gpu 的基础上进一步预装了 Jupyter Notebook 或 JupyterLab。Jupyter 是一个流行的交互式计算环境，常用于数据科学、机器学习和教育领域。这个镜像的目标用户是希望在GPU环境中直接通过浏览器进行代码编写、数据分析、可视化和演示的人员。使用这个镜像时，用户不需要额外安装和配置Jupyter，可以直接启动容器并开始工作。

总结起来，两者的主要差异在于是否预装了 Jupyter 环境。latest-gpu 更偏向于一个基础开发环境，而latest-gpu-jupyter 则是一个更具体的应用环境，为用户提供了一站式的 GPU 加速的数据科学工作平台。

- https://github.com/tensorflow/build/blob/master/tensorflow_runtime_dockerfiles/gpu.Dockerfile

## Installation
- https://www.tensorflow.org/install

- [TensorFlow Docker requirements](https://www.tensorflow.org/install/docker)

For GPU support on Linux, install [NVIDIA Docker](https://github.com/NVIDIA/nvidia-docker) support.

Take note of your Docker version with `docker -v`. Versions earlier than 19.03 require `nvidia-docker2` and the `--runtime=nvidia` flag.
On versions including and after 19.03, you will use the `nvidia-container-toolkit` package and the `--gpus` all flag. 
Both options are documented on the page linked above.

> Note: nvidia-docker v2 uses `--runtime=nvidia` instead of `--gpus all`. `nvidia-docker v1` uses the `nvidia-docker` alias, rather than the `--runtime=nvidia` or `--gpus all` command line flags.

- Check if a GPU is available: `lspci | grep -i nvidia`

- Verify your nvidia-docker installation: `docker run --gpus all --rm nvidia/cuda nvidia-smi`

- Docker link: https://hub.docker.com/r/tensorflow/tensorflow/tags

- Run the jupyter server
```
# tag-jupyter	The specified tag release with Jupyter (includes TensorFlow tutorial notebooks)

docker pull tensorflow/tensorflow:latest-gpu-jupyter

docker run --gpus=all -it -d --rm -v $(realpath ~/notebooks):/tf/notebooks -p 8888:8888 tensorflow/tensorflow:latest-gpu-jupyter

docker ps
```

But when I try to enter the detached docker container with `docker exec -it XX bash`, I get this:
```
WARNING: You are running this container as root, which can cause new files in
mounted volumes to be created as the root user on your host machine.

To avoid this, run the container by specifying your user's userid:

$ docker run -u $(id -u):$(id -g) args...
```

## Mean squared error 与  mean absolute error 有什么不同，分别适合什么情况？
Mean squared error (MSE)和mean absolute error (MAE)是两种常见的用于评估模型预测准确性的度量方法。

- Mean Squared Error (MSE)： MSE是通过计算预测值与实际值之间的差的平方的平均值来衡量误差的。其公式为：MSE = (1/n) * Σ((yi - ŷi)^2)，其中n是样本数量，yi是第i个样本的实际值，ŷi是对应的预测值。MSE对较大的误差惩罚更严重，因为它将误差进行平方，这使得模型对极端误差更为敏感。

- Mean Absolute Error (MAE)： MAE则是通过计算预测值与实际值之间绝对差的平均值来衡量误差的。其公式为：MAE = (1/n) * Σ(|yi - ŷi|)。相比MSE，MAE对误差的惩罚是线性的，不考虑误差的平方，因此在处理异常值或离群点时，MAE可能更具鲁棒性。

适合的情况：
- MSE通常在回归问题中使用，尤其是在对较小误差较为敏感或者预测值和实际值具有高相关性的情况下。由于MSE对大误差的惩罚较大，它鼓励模型尽可能地减小所有误差，包括那些已经相对较小的误差。

- MAE也在回归问题中使用，特别是在对异常值或离群点有较高抵抗力的需求时。因为MAE对误差的惩罚是线性的，所以它对极端误差的敏感度低于MSE。这使得MAE在某些情况下可能提供更稳定、易于解释的性能度量。

总的来说，选择MSE还是MAE取决于具体的应用场景和需求。如果数据中存在大量离群点或者对误差的绝对大小更感兴趣，MAE可能是更好的选择。而如果关注的是整体误差的平均程度，并且数据中没有太多的离群点，MSE可能更适合。

## one-hot encode 是什么？
One-hot encoding 是一种将分类变量转换为数值形式的编码方法。在这种方法中，每个类别被赋予一个唯一的二进制向量，其中只有一个元素（称为“热点”）为1，其余元素均为0。这种编码方式使得每个类别成为一个独立的特征，且这些特征之间互斥。
例如，如果有一个包含三个类别的变量（红色、绿色、蓝色），经过 one-hot encoding 转换后，每个样本会被表示为一个三维向量：
- 红色: (1, 0, 0)
- 绿色: (0, 1, 0)
- 蓝色: (0, 0, 1)

通过这种方式，one-hot encoding 可以处理非连续型数值特征，并在一定程度上扩充了特征空间。
它在机器学习和数据分析中非常常用，特别是在处理分类数据作为输入的算法时，如神经网络和某些类型的回归模型。这种方法有助于算法理解每个类别的独立性，而不会引入潜在的顺序或数值含义到原本无序的类别中。


## Slim Net

- https://github.com/tensorflow/models/blob/master/research/slim/nets/vgg.py

## vgg的原理是什么？
VGG（Visual Geometry Group）是一种深度卷积神经网络（Convolutional Neural Network，CNN）模型，由牛津大学的视觉几何组（Visual Geometry Group）在2014年开发，主要用于大规模图像识别任务。

VGG的原理主要包括以下几点：
- 深度网络架构：VGG探索了构建非常深的网络的可能性，通过堆叠多个卷积层和池化层来提取图像的复杂特征。它的深度可以达到16到19层，这在当时是相当深的网络结构。
- 小卷积核尺寸：VGG使用了小的卷积核尺寸（3x3），这种设计允许网络学习更精细的局部特征，同时减少了参数数量。连续的小卷积核可以模拟大卷积核的效果，但具有更少的参数，有利于防止过拟合。
- 网络稀疏性和表现力：VGG遵循了Hebbian原则的思想，即在网络中越高层的特征应该越抽象和稀疏。通过逐步增加网络深度，VGG能够学习到更高级别的特征表示，从而提高分类性能。
- 连续的下采样：VGG使用最大池化层进行下采样，这些池化层通常在连续的卷积层之间插入，用于减少空间维度并保持特征的稳定性和鲁棒性。
- 全连接层和softmax分类：在卷积和池化层之后，VGG使用全连接层将局部特征映射到全局特征，并通过softmax函数进行多类别的概率输出，实现图像的分类。

总的来说，VGG的原理是通过构建深层的、使用小卷积核的网络结构来学习图像的多层次特征表示，从而提高图像识别的准确性和泛化能力。

## vgg的输入和输出分别是什么？

在VGG网络中：
- 输入：VGG网络的输入通常是经过预处理的图像数据。这些图像通常被调整为固定的尺寸，例如224x224像素，并且每个像素值会被归一化到0-1的范围内。有时，图像还会进行颜色转换，如从RGB色彩空间转换到BGR色彩空间。

- 输出：VGG网络的输出取决于其具体的任务和设计。在最原始的VGG网络（如VGG16）中，输出是一个固定长度的向量，代表了对输入图像属于不同类别的概率分布。例如，在ImageNet分类任务中，输出向量的长度为1000，因为ImageNet数据集包含了约1000个类别。每个输出元素的值表示网络对该图像属于对应类别的预测概率。

需要注意的是，VGG网络也可以被用作特征提取器，在这种情况下，输出将是网络中间某一层的激活值，这些激活值可以作为图像的高级特征表示，用于其他任务，如物体检测、图像分割或风格迁移等。

## 目标检测最初的网络是哪个？
目标检测最初的网络可以追溯到 R-CNN（Region-based Convolutional Neural Network，区域卷积神经网络）。
R-CNN于2014年由Ross Girshick等人提出，它标志着深度学习在目标检测任务中的重要突破。
R-CNN网络通过Selective Search算法生成候选区域（Region Proposals），
然后对每个候选区域进行裁剪并输入到预训练的CNN中提取特征，
最后利用这些特征进行分类和边框回归。

虽然R-CNN在当时取得了显著的效果，但它的计算效率相对较低，因为对每个候选区域都需要单独进行CNN前向传播。
后续的研究工作如Fast R-CNN和Faster R-CNN对其进行了优化和改进。

## VGG输出类别信息，Fast R-CNN 是怎么使用VGG的？
Fast R-CNN是一种深度学习框架，用于对象检测。它利用了预训练的VGG网络作为基础网络。

在Fast R-CNN中，VGG网络主要用于特征提取。
具体来说，首先将输入图像送入VGG网络，经过多层卷积和池化操作后，得到丰富的图像特征图。这些特征图包含了图像中的语义和空间信息。

接下来，Fast R-CNN通过区域提案网络（Region Proposal Network，RPN）生成一系列可能包含对象的候选框。
然后，对每个候选框在特征图上进行RoI（Region of Interest）池化操作，将不同大小和形状的候选框调整为固定尺寸的特征向量。
这些特征向量随后被送入两个并行的全连接层。
其中一个全连接层用于分类，输出类别信息。这里的类别信息就是由VGG网络的最后几层产生的，经过Fast R-CNN的分类器进行进一步处理。另一个全连接层用于边界框回归，以精确定位对象的位置。

总的来说，Fast R-CNN利用VGG网络提取图像特征，并在此基础上进行对象分类和位置回归，实现了高效的物体检测。

##
- https://d2l.ai/chapter_convolutional-modern/vgg.html
- https://www.kaggle.com/code/blurredmachine/vggnet-16-architecture-a-complete-guide

