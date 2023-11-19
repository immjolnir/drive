# Data Structure

当处理一组数据的时候，只有递推（recurrence) 和 递归(recursion) 两种代码能够推动程序不断前行，直到把数据处理完成。

- 穷举法 proof by exhaustion, 也叫蛮力法 brute force method. 

递推思想中的推指立足当下已知数据向着目标结果推导出下一步结果，直到达成目标。
递归思想中的归，即回归的之意，指的是一开始我并不知道当下的结果是什么，需要等待用于构建当下结果的基础结果都手机上来了（即放出去的问题都有了答案，各答案纷纷回归本处），才能得到当下想要的结果。
递归思想的两个独特优势就出现了，一，递归思想可以保证所有底层问题（子问题）一定在上层问题（父问题）解决之前就得到解决，而且通过缓存子问题的结果还可以保证每个子问题只求解一次，不重复求解。
二，这些收集上来的结果可以在当下这步进行“碰撞”，即进行协同、汇总、平衡、取舍。或者说，它自带“对齐”效果。

以：给定一组int[] 类型的数组，请分别用递推和递归的思想对其进行求和。

- Recurrence(recurrent)
```c++
std::array<int, 7> arr {1, 2, 3, 4, 5, 6, 7};

int sum(const std::array<int, 7>& arr) {
	int sum = 0;
	for(auto i: arr )  sum += n;
	return sum;
}
```

- Recursion(recursive) 递归的
```c++
std::array<int, 7> arr {1, 2, 3, 4, 5, 6, 7};

int sum(const std::array<int, 7>& arr) {
	return sumToEnd(arr, 0);
}

int sumToEnd(std::array<int, 7>& arr, int curr) {
	if ( curr == arr.size() - 1 ) return arr[curr];
	return arr[curr] + sumToEnd(arr, cur + 1);
}

```

## 实现方案
- 递推思想
  - 1. 用递推代码实现递推思想：中规中距，使用循环语句，有时候会使用队列（queue）
  - 2. 用递归代码实现递推思想：自顶向下的递归代码，可使代码得到简化

- 递归思想
  - 1. 用递归代码实现递归思想：顺理成章，方法调用自己：自底向上，结果对齐
  - 2. 用递推代码实现递归思想：使用循环语句加栈(stack)，避免了调用栈的溢出

tree，
对tree上的数据，可能想得到tree上额某个极值（最大值，最小值），也坑是tree上的某个统计值（元素的和，叶子的个数），还有可能是tree的某个特性（tree的高度，深度）。
> 所有可以用来创建和表示图的方法都可以用来创建和表示tree.

```c++
class Node {
	int val;
	Node * left;
	Node * right;

	explicit Node(int val_): val(val_) {} 
};
```
解决Question 2的问题， 为了得到结果:
```
        4
       / \
      /   \
     /     \
    2       6
   / \     /  \
  /   \   /    \
 1     3 5      7
```
代码如下:
```c++

std::array<int, 7> arr {1, 2, 3, 4, 5, 6, 7};

// li: low index
// hi: high index
// mi: middle index
static Node* buildTree(const std::array<int, 7> & arr, int li, int hi){
	if (li > hi) return null; // 越界代偿

	int mi = li + (hi - li )/2;
	Node* left_subtree_root = buildTree(arr, li, mi-1);
	Node* right_subtree_root = builtTree(arr, mi+1, hi);
	Node * root = new Node(arr[mi]);
	root->left =  left_subtree_root;
	root->right = right_subtree_root;
	return roo;
}
```

这段代码无论从思想上还是实现上都是利用的标准的“自底向上”式的递归：找到数组某一段上的中点，取中点左边的子段构建左子树，中点右边的子段构建右子树，等他们都构建好以后，再用中点值构建出根结点，并设置左右子树，然后返回这棵树的根结点。
因为中点左右两边子段的长度相差不会大于1，所以构建出来的树一定平衡的。又因为数组是排好序的，所有左子树上的结点一定比根结点小，右子树的结点都大于根结点，满足二叉搜索树的定义。

> 突出“先构建底层子树，再构建上层根结点” 这个“自底向上”的顺序

理解这个思路后，简化代码如下：
```c++
static Node* buildTree(const std::array<int, 7> & arr, int li, int hi){
	if(li > hi) return null;
	int mi = li + (hi - li )/2;
	Node * root = new Node(arr[mi]);
	root->left = buildTree(arr, li, mi - 1);
	root->right = buildTree(arr, mi+1, hi);
	return root;
}
```
虽然这里先构建了根结点，但他并没有改变“直到两个子树都构建完成，当前这个棵树才构建完成”的事实。



### 用递推代码实现递推思想求树的路径和

> 路径，指的是从某一结点到其某一后代结点经过的路线，路径上不包含重复结点。而“路径和”指的是路径上所有结点的val的和。

基本思路：根到当前结点的路径和加上当前结点的值val就是更到当前结点的路径和。如果这个节点没有子节点，则这个路径和就是完整的；否则就把当前的不完全路径和继续向子级结点推进下去。
因为二叉树上子级结点有可能是两个，所有向子级推进的时候有可能产生多个路径和。将这个递推思想用递推式的代码实现出来就是这样的

```c++
int getTotalPathSums(const Node* root) {
	Node * iter = root;
	int sum = 0;
	std::queue<Node*> Q;
	/*
		 back -----------> front
    	.---.----.----.---.---.
   Queue|   |    |    |   |   | 
    	'---'----'----'---'---'
	*/
	Q.emplace_back(root);
	while(!Q.empty()) {
		Node * node = Q.pop_front();
		sum += node->val;
		if(!node->left) Q.emplace_back(node->left);
		if (!node->right) Q.emplace_back(node->right);
	}
	return sum;
}
```
or 
```c++

```


## Question
- 1. 给你一组平面上的点，请你去掉两个点，让剩余的点在平面上所占的面积最小。

- 2. 给定一个有序整数类型数组，如何能够把它转化为一个平衡的(balanced)二叉搜索tree(binary search tree, BST)呢？


## Answer
- 1. 分别把这组点中 x 和 y的值最大和最小的点找出来，一共4个，然后两两组合，看看剩下的面积最小是多少就行了，一共尝试6次。（明智地限定好范围)


