# 8-5

这是一个求最大利润的问题, 我的理解是识别出所有会涨的区段, 过滤掉所有会跌的区段, 然后对会涨的区段求和就是最终结果.

那这个算法肯定是遍历一遍就行了, 也是两个index记录下标, 然后一趟遍历找到所有会涨的区间.

ruby果然用的还是不熟练啊, .each方法和块的结合, 对于遍历数组来说非常方便.