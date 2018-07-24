# 15.1.1 fixed traits

使用特化的模板, 将每个可能用到的T类型与一个用来存储total的类型关联起来, 保证不会溢出.
具体见书15.1.1的例子, 稍稍有点复杂.

总体来说, 就是在创建的时候就指定模板的一些额外特性, 在使用的时候可以不用明确指定特化类型.

# 15.1.4 policy

policy(单词意思为规则,政策)指那些提供算法的类/函数, 或者叫function object.
然后传入模板.

比如STL中的sort()算法, 可以传入比较函数, 传入的比较函数就是policy.

# 15.1.5 trait和policy

书中赞同<Modern C++ design>的观点, policy更加注重于行为, trait更加注重于类型.
然后对两者给出了一些定义, 列在P252中.
- trait表述了模板参数的一些自然的额外属性.
- policy表述了泛型函数和泛型类的一些可配置行为(通常都具有经常被使用的缺省值).


# 15.2.1 确定元素的类型

使用局部特化, 来抽取特定容器的类型.
    template <typename T>
    class ElementT; // 基本模板

    template <typename T>
    class ElementT<std::vector<T>> { // 对vector的局部特化
    public:
        typedef T Type;
    };

    template <typename T>
    class ElementT<std::queue<T>> { // 对queue的局部特化
    public:
        typedef T Type;
    }

    template <typename T>
    void print_element_type(T const& c) // 通过演绎来匹配局部特化, 然后抽取类型
    {
        cout << "Container of "
            << typeid(typename ElementT<T>::Type).name()
            << " elements.\n";
    }

    int fun()
    {
        vector<bool> a;
        print_element_type(a); // 打印类型
    }

# 15.2.2 确定class类型

STL中有提供std::is_class<>()函数, 本节使用的函数在vs2015中编译不过.
但是这种思想还是可以了解一下的.

# 15.2.4 promotion trait

记录一个非常有用的类型模板, 在本节也有用到.
    template <bool C, typename Ta, typename Tb>
    class IfThenElse;

    template <typename Ta, typename Tb>
    class IfThenElse<true, Ta, Tb>
    {
    public:
        typedef Ta ResultT;
    }

    template <typename Ta, typename Tb>
    class IfThenElse<false, Ta, Tb>
    {
    public:
        typedef Tb ResultT;
    }

然后在选择类型时, 就可以使用比如下面这样的方式
    typedef template IfThenElse<sizeof(xxx) > sizeof(yyy), xxx, yyy>::ResultT Type;

这个模板不会将Ta和Tb都实例化, 如果只用到Ta就实例化Ta.
这一点在传入的是两个模板实例化个体时非常重要, 能够减少实例化的模板数量.