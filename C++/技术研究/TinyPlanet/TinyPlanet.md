# 8-22

当时实现TinyPlanet时, 没有理解他的原理, 只是抄了代码, 现在再来回顾一下, 并且整理出原理吧.

算法的实现代码集中在PanoramaView.cpp的`g_fnTinyPlanet()`函数中.
算法的核心就是对每个点的坐标进行转换, 然后调用opencv的remap函数做一次映射.

实现的时候是找github上的代码翻译的, 刚找了一份算法说明, 将的挺不错的
	https://math.stackexchange.com/questions/607353/tiny-planet-algorithm

tinyplanet是一张极坐标图, 对于上面每一个点要找到他们在原图上的坐标.
找这个坐标的过程就是极坐标转换, 也就是本算法的核心内容啦.

