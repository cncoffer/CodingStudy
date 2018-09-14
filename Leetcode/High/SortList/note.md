# 8-28

链表排序, 实际上手动实现一下快排就行了吧.
正好温习一遍快排.

仔细思考了一下, 并不能使用快排, 因为后面的节点没有指向前一个节点的指针.

# 算法一

考虑到上面使用快排来实现, 那可以使用数组将数据存下来, 然后对数组进行快排. 最后将结果拷贝回去.
先简单的实现一下吧.

# 算法二

题目还要求使用常数空间.
那就意味着只能使用原地排序算法.
这样的话考虑使用分治法.
思路是这样的
1. 首先将链表从中间截断, 对左右两边分别进行排序.
2. 然后将左右两边已经排序好了的链表merge.

这样时间复杂度应该是o(nlogn)了.