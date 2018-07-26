# 17 metaprogram

metaprogram的概念是系统执行metaprogram来生成新的代码, 而这些新的代码才真正实现了我们所期望的功能.
这里的执行指编译期的执行, 因此能够带来运行期开销的优化. 其实就是把用户想要的计算放到了编译期来做(不过实例化模板时空间开销会增加).

# 17.1 编译期计算幂

这里用到了16.3节的CRTP, 原来CRTP还挺有用的啊.

下面是计算3的幂的例子
    template <int N>
    class Pow3 {
    public:
        enum { result = 3 * Pow3<N - 1>::result };
    };

    // 特化的模板作为递归终点
    template <>
    class Pow3<0> {
    public:
        enum { result = 1 };
    };

另外, 静态成员变量和枚举值的用法也类似, 将result声明为静态常量也能实现上面功能.

# 17.5

metaprogram在实际应用中使用不多, 因为很多时候他需要递归很多次, 也就意味着会嵌套实例化模板非常多层.
而C++标准建议只进行17层以内的递归实例化.
不过, 对于某些性能要求很严格的算法, 可以大大提高其效率.
讲道理, 可以看看tiny planet有没有可能用metaprogram再来提高一下效率呢.

# 17.7 使用metaprogram来展开循环

本节的例子实现的是向量点乘的展开.
    template <int DIM, typename T>
    class DotProduct {
    public:
        static T result(T *a, T *b) {
            return *a * *b + DotProduct<DIM - 1, T>::result(a + 1, b + 1);
        }
    };

    // 特化的模板, 作为递归终点.
    template <typename T>
    class DotProduct<1, T> {
    public:
        static T result(T *a, T *b) {
            return *a * *b;
        }
    };

    template <int DIM, typename T>
    inline T dot_product(T* a, T* b)
    {
        return DotProduct<DIM, T>::result(a, b);
    }

比如
    int a[] = {1,2,3};
    int b[] = {5,6,7};
    dot_product<3>(a, b);

等价于`a[0]*b[0] + a[1]*b[1] + a[2]*b[2]`
展开后编译器能够进行更好的优化, 也就意味着更好的性能.

