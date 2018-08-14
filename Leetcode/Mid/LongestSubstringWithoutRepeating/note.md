# 8-12

给定一个字符串, 找出不含重复字符的最长子串的长度.

# 算法一

创建一个长度为26int的数组, 用于保存各个字母的index, 全部初始化为-1.
接着设定一前一后两个下标为left, right, 分别指向子串的左侧和右侧. 用左闭右闭区间会比较方便处理.
然后不停的移动右right, 每遇到一个字符, 就去检查一下上面的数组中这个字符是否在left右侧, 如果在就将left移动到数组中下标的右侧.
并且更新数组中字符的下标和最大长度.
直到right走到字符串末尾.

这样时间复杂度为o(n).

好吧, 这题是字符不单单只a-z的字母, 所以数组的大小要增大.