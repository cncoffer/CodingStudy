# 条款49 了解new-handler的行为

当operator new抛出异常以反映内存分配失败之前, 它会先调用一个客户指定的错误处理函数, new-handle.
new-handle的使用必须先用`set_new_handler`指定.

set_new_handler, get_new_handler都定义在<new>头文件中, 具体可以自行查看.

现在的new在分配失败时是抛出异常, 而因为以前的c++是要求返回null, 所以如果想用旧形式来处理, 可以使用`new(std::nothrow)`版本. 但并没有啥意义, 因为这里不抛异常其他地方还是有可能抛异常的...

# 条款50-52 new和delete

讲的都是自己实现new和delete时需要做的, 没兴趣看.