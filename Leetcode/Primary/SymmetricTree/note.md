# 8-11

给定一个二叉树, 判断是否为左右对称的.
最好分别使用递归和迭代来做.

# 算法一

那这个时候层序遍历就排上用场了, 在层序遍历的同时, 将同一层的node丢到一个deque中, 遍历完这一层后, 判断deque中的这一层是否是对称的.

这里需要注意, null指针也必须放到队列中. 不然下面这种会被错误的判断为true.
    1
   / \
  2   2
   \   \
   3    3

仔细想了一下, 保证吧null指针也塞到队列里肯定没有问题了, 我就懒得实现了. 思考下算法二怎么用迭代来实现吧.

# 算法二

想了一下, 用递归似乎更简单, 首先判断左节点和右节点是否相等.
然后递归判断左节点的左子树和右节点的右子树是否相等.
以及左节点的右子树和右节点的左子树是否相等.

搞定, 吃完饭来用层序遍历或者迭代方法实现一下.

# 算法三

树的遍历如果要做迭代的话就是用栈, 因为栈的先入后出的特性, 要先放右节点再放左节点.
然后对于本问题的话, 就是左半边的先放右节点后放左节点, 右半边是先放左节点再放右节点.

然后调试的时候想自己实现一棵树, 发现有点麻烦, 那就学习了一下leetcode的代码.
他是用deque来实现, 首先将根节点放入queue, 然后在循环中是这样做的:
- 取出queue.front()
- 对其左节点赋值(并将左节点放入queue)
- 如果还有输入对其右节点赋值(并将右节点放入queue)
- 然后下一个循环

这样就能根据输入[1,2,2,3,4,4,3]来创建一颗形如下面这样的树了.
    1
   / \
  2   2
 / \ / \
3  4 4  3

实现代码贴在下面了
    void trimLeftTrailingSpaces(string &input) {
        input.erase(input.begin(), find_if(input.begin(), input.end(), [](int ch) {
            return !isspace(ch);
        }));
    }

    void trimRightTrailingSpaces(string &input) {
        input.erase(find_if(input.rbegin(), input.rend(), [](int ch) {
            return !isspace(ch);
        }).base(), input.end());
    }

    TreeNode* stringToTreeNode(string input) {
        input = input.substr(1, input.length() - 2);
        if (!input.size()) {
            return nullptr;
        }

        string item;
        stringstream ss;
        ss.str(input);

        getline(ss, item, ',');
        TreeNode* root = new TreeNode(stoi(item));
        queue<TreeNode*> nodeQueue;
        nodeQueue.push(root);

        while (true) {
            TreeNode* node = nodeQueue.front();
            nodeQueue.pop();

            if (!getline(ss, item, ',')) {
                break;
            }

            trimLeftTrailingSpaces(item);
            if (item != "null") {
                int leftNumber = stoi(item);
                node->left = new TreeNode(leftNumber);
                nodeQueue.push(node->left);
            }

            if (!getline(ss, item, ',')) {
                break;
            }

            trimLeftTrailingSpaces(item);
            if (item != "null") {
                int rightNumber = stoi(item);
                node->right = new TreeNode(rightNumber);
                nodeQueue.push(node->right);
            }
        }
        return root;
    }