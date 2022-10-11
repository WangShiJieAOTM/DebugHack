## 函数调用时的参数传递方法(x86_64)

#### 函数参数与调试

```shell
$ ./print
a: 100, b: 35000, c: 5, d: A, e: 123456789
f: 3.140e+00, g: 2.998e+08
h: 0x5586b826a010, i: 0x5586b826a014, j: 0x5586b8268053
```

#### 通过GDB确认

```shell
(gdb) b func
Breakpoint 1 at 0x55555555517b: file /home/aotm/DebugHack/Parameter_transfer_in_function_call_x86/demo.c, line 8.
(gdb) r
Breakpoint 1, func (a=100, b=35000, c=5, d=65 'A', e=123456789, f=3.1400001, g=299792458, h=0x555555558010 <v1>, i=0x555555558014 <v2>, j=0x555555556053 "Hello") at /home/aotm/DebugHack/Parameter_transfer_in_function_call_x86/demo.c:8
8           printf("a: %d, b: %ld, c: %d, d: %c, e: %lld\n" "f: %.3e, g: %.3e\nh: %p, i: %p, j: %p\n", a, b, c, d, e, f, g, h,
```

如果编译选项没有 -g 且 CmakeList编译选项不是Debug，那么GDB无法显示参数的值。

#### x86_64下的调用

在 x86_64中,整型和指针型参数会从左到右依次保存到
rdi,rsi,rdx,rcx,r8,r9寄存器中,浮点型参数会从左到右依次保存到xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7寄存器中,剩余的参数会从右到左依次压栈。

在该状态下确认寄存器的内容,可以使用如下命令:

``` shell
(gdb) i r
rax            0x41                65
rbx            0x0                 0
rcx            0x41                65  ## 参数d
rdx            0x41                65  ## 参数c
rsi            0x88b8              35000  ## 参数b
rdi            0x64                100  ## 参数a
rbp            0x7fffffffe3a0      0x7fffffffe3a0
rsp            0x7fffffffe370      0x7fffffffe370
r8             0x75bcd15           123456789  ##参数e
r9             0x555555558010      93824992247824  ##参数h
r10            0x7ffff7fc3908      140737353890056
r11            0x7ffff7fde680      140737354000000
r12            0x7fffffffe4d8      140737488348376
r13            0x5555555551da      93824992235994
r14            0x555555557dc0      93824992247232
r15            0x7ffff7ffd040      140737354125376
rip            0x55555555517b      0x55555555517b <func+50>
eflags         0x202               [ IF ]
cs             0x33                51
ss             0x2b                43
ds             0x0                 0
es             0x0                 0
fs             0x0                 0
gs             0x0                 0
```

此外,浮点型的第6,第7个参数f,g分别保存到了xmm0,xmm1寄存器中,可以使用如下命令查看:

``` shell
(gdb) p $xmm0.v4_float[0]
$1 = 3.1400001
(gdb) p $xmm1.v4_float[0]
$2 = 2.9979246e+08
```

之所以要给xmm0/xmm1加上后缀(v4_float,v2_double)的原因是,GDB将这些寄存器看做下面的联合:

```c
union{
    float v4_float[4];
    double v2_double[2];
    int8_t v16_int8[16];
    int16_t v8_int16[8];
    int32_t v4_int32[8];
    int64_t v2_int64[8];
    int128_t unit128;
}xmm0;
```

这是因为,xmm0和xmm1的实际长度为128比特,但也同时能够保存更小的变量.

指针类型的第8个参数h的处理方法和整型相同,保存到r9中,由于第9,第10个参数也是指针,参数少的情况会被保存到寄存器中,但在本hack的例子中,寄存器不够用,因此这两个参数被保存到栈中进行传递


查看栈的方法如下所示,虽然现在我们只关心两个参数,但却需要显示3个,这是因为栈的开头保存了函数的返回地址,另外,用g(giant word)双字格式显示的原因是,在x86_64架构中,整数和指针的大小就是 giant word

``` shell   
(gdb) x/3g $rsp
0x7fffffffe3a8: 0x000055555555522e      0x0000555555558014
0x7fffffffe3b8: 0x0000555555556053
```

因为有时我们需要知道指针指向的值或字符串,我们可以使用如下命令:

``` shell
(gdb) printf "%.2f\n",*(float*)0x0000555555558014
0.01
(gdb) p (char*)0x0000555555556053
$2 = 0x555555556053 "Hello"
(gdb) 
```




