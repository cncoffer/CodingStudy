# 8-15

这个合并区间的话就遍历一遍就行了呀, 似乎没什么算法嘛, 实现的时候再记录难点吧.

本来以为是按照从小到大排序的, 不过没关系, 这样就先给他排个序呗.
另外还需要注意, 上面的排序只保证了左区间是递增的, 但是右区间不能保证, 所以合并的时候要通过比较来决定右边界.