# 8-11

这题不就和刚才做的最大收益那题一样嘛.

# 算法一

首先分解问题
    sum(n) = max(nums[n], sum(n-1) + nums[n])
    sum(0) = nums[0]

接着就是实现啦.