## 函数调用时的参数传递方法(C++)

#### C++语言的函数调用

```C++
class foo {
    int a;
    int b;
public:
    void func(int x, int y);
};

void foo::func(int x, int y) {
    a = x;
    b = y + 2;
}

int main() {
    foo f1, f2;
    printf("f1:%p,f2:%p\n", &f1, &f2);
    f1.func(5, 1);
    f2.func(-4, 2);
    return 0;
}
```

这里调用函数的时候把5,1通过函数传递给了func 但是在执行f1.func(5,1)的时候要明确传递给f1的a和b

实际上func(int x,int y)收到了要访问的实例的信息,也就是func()的行为相当于下述C语言函数,因此调用方法时,传递的参数要比原型声明的参数多一个

```C++
void func(foo *this, int x, int y) {
    this->a = x;
    this->b = y + 2;
}
```

在编译后的ELF文件中,无论是C++函数还是C函数,甚至是汇编语言函数,它们之间没有任何区别,但是,C++函数会在编译时经过函数名变换,

```shell
nm demo |grep foo|grep func
000000000000116a T _ZN3foo4funcEii
```

可以看到,编译后的函数名变成了`_ZN3foo4funcEii`这样的形式,这是C++编译器为了区分不同的函数而做的变换,这个变换的过程叫做名字修饰

如果要显示恢复的函数可以用:

```shell
nm demo |grep foo|grep func|c++filt
000000000000116a T foo::func(int, int)
```

相同的:

```shell
nm -C demo|grep foo|grep func
000000000000116a T foo::func(int, int)
```

#### 在x86_64中查看参数

```shell
gdb demo
(gdb) b *_ZN3foo4funcEii
或
(gdb) b *foo::func(int, int)
Breakpoint 1 at 0x116a: file /home/aotm/DebugHack/Parameter_transfer_in_function_call_cpp/demo.h, line 13.
(gdb) r
Breakpoint 1, foo::func (this=0x10101000000, x=32767, y=-134471680) at /home/aotm/DebugHack/Parameter_transfer_in_function_call_cpp/demo.h:13
13      void foo::func(int x, int y) {
(gdb) i r
rax            0x7fffffffe3a8      140737488348072
rbx            0x0                 0
rcx            0x1                 1
rdx            0x1                 1
rsi            0x5                 5
rdi            0x7fffffffe3a8      140737488348072
```

在i386中查看参数

```shell
gdb demo
(gdb) b *_ZN3foo4funcEii
或
(gdb) b *foo::func(int, int)
Breakpoint 1 at 0x116a: file /home/aotm/DebugHack/Parameter_transfer_in_function_call_cpp/demo.h, line 13.
(gdb) r
Breakpoint 1, foo::func (this=0x10101000000, x=32767, y=-134471680) at /home/aotm/DebugHack/Parameter_transfer_in_function_call_cpp/demo.h:13
13      void foo::func(int x, int y) {
(gdb) x/3 $esp+4
0x7fffffffe3a8: 0x00000005      0x00000001      0x00000000
(gdb) c
(gdb) x/3 $esp+4
0x7fffffffe3a8: 0x00000005      0x00000001      0x00000000
```