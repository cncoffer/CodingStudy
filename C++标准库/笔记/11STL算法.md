### 11 STL算法 ###

因为很难在规定时间内看完了, 那就挑着看吧, 对于我不了解的内容先看, 其他的用到的时候查吧.

STL的算法定义在<algorithm>
数值处理的STL算法定义在<numeric>
function object和function adapter定义在<functional>

### 11.4 for_each() ###

for_each()在C++11, 可以被range-based for完全的取代, 而且后者的行为更加自然.

### 11.6.5 更易型算法-赋值 ###

这正是我找了很久的序列化啊!

赋常量值:
	void fill(ForwardIterator beg, ForwardIterator end,
			const T& newValue);
	void fill_n(ForwardIterator beg, Size num,
			const T& newValue);

fill()将区间[beg,end)内的每个元素赋予值newValue.
fill_n()将从beg开始的前num个元素赋予值newValue.

赋生成值:
	void generate(ForwardIterator beg, ForwardIterator end, 
			Func op);
	void generate_n(ForwardIterator beg, Size num,
			Func op);

generate()会调用op()产生新值, 并将它赋予[beg,end)内每个元素.
generate_n()同理.

于是可以使用generate()和function object产生一串自己想要的数值, 比如01序列, 奇数序列等等. 比如生成01序列可以这样写
	class MyAssign
	{
	public:
		MyAssign() : m_nNum(0) {};
		int m_nNum;
		int operator()() {
			int ret = m_nNum;
			m_nNum = !m_nNum;
			return ret;
		}
	};
	
	int fun()
	{
		list<int> coll;
	
		generate_n(back_inserter(coll), 10, MyAssign());
	}

另外还有C++11自带的递增赋值
	void iota(ForwardIterator beg, ForwardIterator end,
			T startValue);

这里需要注意, atoi和iota的区别, 两个名字非常像, 前者是将字符串转换为整数, 后者是STL里的递增赋值.

### 11.10 已排序区间算法 ###

原来stl中就有二分查找, 我tm每次想用的时候还傻傻的自己去实现...
注意, 下面这些调用的前提都是容器已经排序了的, 如果未排序结果和复杂度都无法保证.
	bool binary_search(...) // 检查是否存在, 复杂度为对数
	bool includes(...) // 检查数个元素是否存在, 复杂度为线性
	ForwardIterator lower_bound(...) // 查找第一个可能位置
	ForwardIterator upper_bound(...) // 查找最后一个可能位置

	pair<ForwardIterator, ForwardIterator>
	equal_range(...) // 查找相等区间, 与make_pair(lower_bound(), upper_bound())等效


另外还有合并算法, 也有STL的实现.


