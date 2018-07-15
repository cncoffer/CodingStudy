### 15 以string完成I/O ###

iostream用到的很多, 但要说了解, 我真是知之甚少啊. 这节是需要重点查看的.

### 15.1 I/O Stream的共通基础 ###

C++I/O有stream完成. 所谓stream就是一条数据流, 字符序列在其中"川流不息". 按照面向对象原则, stream是由某个class定义出来具有特定性质的对象. 

最重要的两个class是:
class istream定义input stream, 用来读数据.
class ostream定义output stream, 用来写数据.

IOSteam程序库定义了数个类型为istream和ostream的全局对象, 他们对应于标准I/O通道:
cin, 隶属于istream, 是供使用者输入用的标准输入通道, 对应于c中的stdin. 操作系统通常将它和键盘连接.
cout, 隶属于ostream, 是供使用者输出用的标准输出通道. 对应于c中的stdout. 操作系统通常将它和显示器连接.
cerr, 隶属于ostream, 对应于c中的stderr. 默认情况下cerr无缓冲.
cerr, 隶属于ostream, c中没有对应. 默认情况下连接与cerr所连接的设备, 有缓冲.

### 15.1.15 操控器 ###

很多时候, 我们用std::endl来表示换行, 这东西就被称为操控器.
操控器通常只会改变"输入的解释方式"或"输出的格式化方式", 例如进制.

操控器endl做了两件事
1. 输出换行符, '\n'
2. 刷新output缓冲区, 即应用flush()强制输出缓冲区内所有数据.

下面是一些常用的操控器
- endl : ostream, 输出'\n'并刷新output缓冲区.
- ends : ostream, 输出'\0'.
- flush: ostream, 刷新output缓冲区.
- ws   : istream, 读入并忽略空白字符.

15.6节有对于操控器的更多讨论.

### 15.2.2 全局性的stream对象 ###

自C++11开始, 全局性的stream对象15.1中有提到有了若干关于并发的保证.
当对标准c stream同步化时, 在多个并行线程中使用它们并不会引发不确定的行为. 但是可能会导致字符交错, 或者读到不确定的字符.
而其他的stream对象, 或不与c stream同步化的对象, 并发读写行为不确定.

### 15.3 标准的stream操作符>>和<< ###

