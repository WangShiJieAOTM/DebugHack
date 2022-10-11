## 调试时必须的栈知识

sum.c:

```shell
$ gcc -o sum -g sum.c
$./sum 10
```

#### 函数调用和栈的关系

```asm
(gdb) disas main
   0x0000555555555296 <+180>:   mov    $0x0,%edi   ##1.把参数压入栈中
   0x000055555555529b <+185>:   call   0x5555555551a9 <sum_till_MAX> ##2.把返回地址压入栈中
   0x00005555555552a0 <+190>:   mov    %rax,-0x8(%rbp)  ##3.栈顶指针移位
   
(gdb) disas sum_till_MAX
   0x00005555555551a9 <+0>:     endbr64 
   0x00005555555551ad <+4>:     push   %rbp  ##4.保存上层帧的帧指针
   0x00005555555551ae <+5>:     mov    %rsp,%rbp  ##5.将新的栈帧赋给帧指针
   0x00005555555551b1 <+8>:     sub    $0x20,%rsp  ##6.在栈上分配用于保存自动变量的空间
   0x00005555555551b5 <+12>:    mov    %edi,-0x14(%rbp)  ##相当于u64 sum;
=> 0x00005555555551b8 <+15>:    addl   $0x1,-0x14(%rbp)  ##相当于 n++;
   0x00005555555551bc <+19>:    mov    -0x14(%rbp),%eax
   0x00005555555551bf <+22>:    mov    %rax,-0x8(%rbp)  ##8.因为sum是u64,n是u32,所以只要后面的位数  这一句相当于 15行 sum = n
   0x00005555555551c3 <+26>:    mov    0x2e47(%rip),%eax        # 0x555555558010 <max_addend>
   0x00005555555551c9 <+32>:    cmp    %eax,-0x14(%rbp)
   0x00005555555551cc <+35>:    jae    0x5555555551dc <sum_till_MAX+51>
   0x00005555555551ce <+37>:    mov    -0x14(%rbp),%eax
   0x00005555555551d1 <+40>:    mov    %eax,%edi
   0x00005555555551d3 <+42>:    call   0x5555555551a9 <sum_till_MAX>
   0x00005555555551d8 <+47>:    add    %rax,-0x8(%rbp)
   0x00005555555551dc <+51>:    mov    -0x8(%rbp),%rax
   0x00005555555551e0 <+55>:    leave  ## 9.删除栈帧,执行和4,5相反的操作,释放当前的栈
   0x00005555555551e1 <+56>:    ret    ## 10.从栈中弹出返回地址,并跳转到该地址
```

#### 调试器的backtrace

GDB等调试器的backtrace功能是通过搜索栈中保存的信息来实现的

```shell 
(gdb) bt
#0  sum_till_MAX (n=2) at /home/aotm/DebugHack/Stack_Acknowledge_in_Debug/sum.c:18
#1  0x00005555555551d8 in sum_till_MAX (n=1) at /home/aotm/DebugHack/Stack_Acknowledge_in_Debug/sum.c:18
#2  0x00005555555552a0 in main (argc=2, argv=0x7fffffffe4f8) at /home/aotm/DebugHack/Stack_Acknowledge_in_Debug/sum.c:31
```

下面来手动执行一下backtrace的过程

```shell
(gdb) i r rip rbp
rip            0x5555555551ce      0x5555555551ce <sum_till_MAX+37>
rbp            0x7fffffffe350      0x7fffffffe350
```

查看栈的内容

```shell
(gdb) x/40w $sp
```

#### 使用GDB操作栈帧

```shell
(gdb) bt
#0  sum_till_MAX (n=4) at /home/aotm/DebugHack/Stack_Acknowledge_in_Debug/sum.c:18
#1  0x00005555555551d8 in sum_till_MAX (n=3) at /home/aotm/DebugHack/Stack_Acknowledge_in_Debug/sum.c:18
#2  0x00005555555551d8 in sum_till_MAX (n=2) at /home/aotm/DebugHack/Stack_Acknowledge_in_Debug/sum.c:18
#3  0x00005555555551d8 in sum_till_MAX (n=1) at /home/aotm/DebugHack/Stack_Acknowledge_in_Debug/sum.c:18
#4  0x00005555555552a0 in main (argc=2, argv=0x7fffffffe4f8) at /home/aotm/DebugHack/Stack_Acknowledge_in_Debug/sum.c:31
```

用frame命令查看现在选择的帧

```shell
(gdb) frame
#0  sum_till_MAX (n=4) at /home/aotm/DebugHack/Stack_Acknowledge_in_Debug/sum.c:18
18              sum += sum_till_MAX(n);
```

现在选中的帧为#0,查看该帧内的自动变量sum,其值为4.

```shell
(gdb) p sum
$1 = 4
```

接下来选择上一层的#1帧

```shell
(gdb) frame 1
#1  0x00005555555551d8 in sum_till_MAX (n=3) at /home/aotm/DebugHack/Stack_Acknowledge_in_Debug/sum.c:18
18              sum += sum_till_MAX(n);
(gdb) p sum
$2 = 3
```

选择帧还可以使用up和down命令来进行

```shell
(gdb) up
#2  0x00005555555551d8 in sum_till_MAX (n=2) at /home/aotm/DebugHack/Stack_Acknowledge_in_Debug/sum.c:18
18              sum += sum_till_MAX(n);
(gdb) p sum
$3 = 2
```

使用info命令的frame选项能够看到更详细的栈帧信息

```shell
(gdb) i frame 1
Stack frame at 0x7fffffffe360:
 rip = 0x5555555551d8 in sum_till_MAX (/home/aotm/DebugHack/Stack_Acknowledge_in_Debug/sum.c:18); saved rip = 0x5555555551d8
 called by frame at 0x7fffffffe390, caller of frame at 0x7fffffffe330
 source language c.
 Arglist at 0x7fffffffe350, args: n=3
 Locals at 0x7fffffffe350, Previous frame's sp is 0x7fffffffe360
 Saved registers:
  rbp at 0x7fffffffe350, rip at 0x7fffffffe358
```

#### 栈大小的限制

实际上,本hack使用的示例程序如果不带参数执行,会引发 segmentation fault

```shell
$ ./sum
Segmentation fault (core dumped)
```

此处发生了栈溢出(stack overflow),通过GDB执行示例程序,可以查看是什么时候出现了segmentation fault,查看程序计数器(PC)即可看到程序执行位置

```shell
(gdb) r
Starting program: /home/aotm/DebugHack/Stack_Acknowledge_in_Debug/build/sum 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".

Program received signal SIGSEGV, Segmentation fault.
0x00005555555551b5 in sum_till_MAX (n=<error reading variable: Cannot access memory at address 0x7fffff7feffc>) at /home/aotm/DebugHack/Stack_Acknowledge_in_Debug/sum.c:12
12      u64 sum_till_MAX(u32 n) {

(gdb) x/i $pc
=> 0x5555555551b5 <sum_till_MAX+12>:    mov    %edi,-0x14(%rbp)
```

发现出现段错误的代码是将 sum_till_MAX()的参数 n PUSH到栈顶端的命令,现在查看一下栈指针(SP)的位置

```shell
(gdb) p $sp
$1 = (void *) 0x7fffff7feff0
```

下面查看该进程的内存映射,要查看GDB attath了进程的内存映射,可以执行下面的命令

```shell
(gdb) i proc mappings
process 3594
Mapped address spaces:
          Start Addr           End Addr       Size     Offset objfile
      0x555555554000     0x555555555000     0x1000        0x0 /home/aotm/DebugHack/Stack_Acknowledge_in_Debug/build/sum
      0x555555555000     0x555555556000     0x1000     0x1000 /home/aotm/DebugHack/Stack_Acknowledge_in_Debug/build/sum
      0x555555556000     0x555555557000     0x1000     0x2000 /home/aotm/DebugHack/Stack_Acknowledge_in_Debug/build/sum
      0x555555557000     0x555555558000     0x1000     0x2000 /home/aotm/DebugHack/Stack_Acknowledge_in_Debug/build/sum
      0x555555558000     0x555555559000     0x1000     0x3000 /home/aotm/DebugHack/Stack_Acknowledge_in_Debug/build/sum
      0x7ffff7d88000     0x7ffff7d8b000     0x3000        0x0 
      0x7ffff7d8b000     0x7ffff7db3000    0x28000        0x0 /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7db3000     0x7ffff7f48000   0x195000    0x28000 /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7f48000     0x7ffff7fa0000    0x58000   0x1bd000 /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7fa0000     0x7ffff7fa4000     0x4000   0x214000 /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7fa4000     0x7ffff7fa6000     0x2000   0x218000 /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7fa6000     0x7ffff7fb3000     0xd000        0x0 
      0x7ffff7fbc000     0x7ffff7fbe000     0x2000        0x0 
      0x7ffff7fbe000     0x7ffff7fc2000     0x4000        0x0 [vvar]
      0x7ffff7fc2000     0x7ffff7fc3000     0x1000        0x0 [vdso]
      0x7ffff7fc3000     0x7ffff7fc5000     0x2000        0x0 /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7fc5000     0x7ffff7fef000    0x2a000     0x2000 /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7fef000     0x7ffff7ffa000     0xb000    0x2c000 /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7ffb000     0x7ffff7ffd000     0x2000    0x37000 /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7ffd000     0x7ffff7fff000     0x2000    0x39000 /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7fffff7ff000     0x7ffffffff000   0x800000        0x0 [stack]
```

我们看到 $sp指针的地址为0x7fffff7feff0 而栈顶的指针地址为0x7fffff7ff000,所以这里发生了stack overflow

**需要注意的是,使用这个命令调用了GDB打开了/proc/<PID>/maps,因此在分析core dump文件的时候无法使用该命令,这时候就需要使用以下命令:**

```shell
(gdb) info files
或者
(gdb) info target)
```

使用 ulimit -s 命令可以查看当前的设置的栈大小

```shell
$ ulimit -s
8192
```

如果将这个值增加10倍,在默认情况下(不传入参数),不会发生 segmentation fault 而正常结束.

```
$ ulimit -s 81920
$ ulimit -s
81920
$ ./sum
sum(0..1048576) = 549756338176
```