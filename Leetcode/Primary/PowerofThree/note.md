# 8-12

判断一个数是否是3的幂.
是否能不使用循环或者递归来做.

# 算法一

使用循环除以3的方法, 如果除到结果为1且没有产生余数即为3的幂.

# 算法二

不用循环的话想到的是stl中的开根号函数, 然后用所得结果来计算3^ret是否等于给定值来判断.

STL中的log函数精度不够高啊...