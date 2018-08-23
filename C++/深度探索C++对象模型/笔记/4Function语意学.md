# 8-23

C++支持三张类型的member functions: static, nonstatic和virtual.
每一种类型被调用的方式各不相同.

# 4.1 member的各种调用方式

## nonstatic member functions(非静态成员函数)

编译器会将nonstatic member functions转换为nonmember形式.
这时就会将函数名改写成一种内部名称, 比如将类目和参数都加到函数名上保证唯一.

## virtual member function(虚拟成员函数)

如果normalize()是一个虚成员函数, 那么一下调用
    ptr->normalize();
会被内部转化为
    (*ptr->vptr[1])(ptr);

vptr表示由编译器产生的指针, 指向virtual table.
1是virtual table slot的索引值, 关联到normalize()函数.
第二个ptr表示this指针.

## static member functions(静态成员函数)

静态成员函数会被转换为一般的nonmember函数调用.
静态成员函数的主要特性就是它没有this指针, 以下的次要特性统统根源于其主要特性:
- 它不能够直接存取其class中的nonstatic members
- 它不能够被生命为const, volatile或virtual.
- 它不需要经由class object才能调用.

静态成员函数可以直接用类名限定符调用, 当然也可以用class object调用.

静态成员函数由于缺乏this指针, 因此差不多等同于nonmember function. 这也带来了一个好处, 作为callback函数.

# 4.2 Virtual Member Functions
