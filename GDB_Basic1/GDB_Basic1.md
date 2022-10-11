## GDB的基本使用方法(一)

(1) 带着调试选项编译,构建调试对象

(2) 启动调试器(GDB)

(2-1)设置断点

(2-2)显示栈帧

(2-3)显示值

(2-4)继续执行

### 准备

通过gcc的-g选项来生成调试信息:

```shell
$ gcc -Wall -O2 -g 源文件
```

**-wall:** 显示警告信息

**-O2:** 优化等级

**-g:** 生成调试信息

如果使用Makefile构建,则在Makefile中添加:

```makefile
CFLAGS = -Wall -O2 -g
```

如果使用configure脚本构建,可以这样用:

```shell
$ ./configure CFLAGS="-Wall -O2 -g"
```

构建方法通常会写在INSTALL,README等文件中.

### 启动调试器

```shell
$ gdb 调试对象
```

### 设置断点

```shell
(gdb) break 函数名
(gdb) break 行号
(gdb) break 文件名:行号
(gdb) break 文件名:函数名
(gdb) break +偏移量
(gdb) break -偏移量
(gdb) break *地址
```

break可以简写为b

[例]

```shell
(gdb)b iseq_complement
(gdb)b complete.c:516
(gdb)b b +3
(gdb)b 0x08116fd6
```

如果只写一个b则在下一行设置断点

设置好的断点可以用info breakpoints查看

### 运行

用 run命令开始运行,不加参数只执行 run,就会执行到第一个断点处后暂停运行,可以简写为r

经常的操作是在main()上设置断点,然后执行到 main()函数,start命令能达到同样的效果

```shell
(gdb) start
(gdb) run -a
```

### 显示栈帧

backtrace命令可以在遇到断点而暂停执行时像是栈帧,命令简写为bt,别名也为where和info stack(简写为 info s)

显示所有栈帧:

```shell
(gdb)backtrace
(gdb) bt
```

只显示开头N个栈帧:

```shell
(gdb)backtrace N
(gdb) bt N
```

只显示最后几个栈帧:

```shell
(gdb)backtrace -N
(gdb) bt -N
```

不仅显示backtrace,还要显示局部变量,N与前述意思相同,表示开头N个(或最后N个)栈帧:

```shell
(gdb)backtrace full
(gdb) bt full
(gdb)backtrace full N
(gdb) bt full N
(gdb)backtrace full -N
(gdb) bt full -N
```

### 显示变量:

print命令可以显示变量的值,简写为p

```shell
(gdb) print 变量名
(gdb) p 变量名
```

### 显示寄存器

info registers命令可以显示寄存器的值,简写为info r(info reg)

```shell
(gdb) info registers
(gdb) info r
```

同样print也能print寄存器的值,只需要在变量名前加上$符号

```shell
(gdb) print $eax
(gdb) p $eax
```

print/p在显示的时候可以使用以下格式:
格式:
p/格式 变量名


| 格式 | 说明                                      |
| :--- | :---------------------------------------- |
| d    | 十进制                                    |
| u    | 无符号十进制                              |
| o    | 八进制                                    |
| x    | 十六进制                                  |
| t    | 二进制                                    |
| f    | 浮点数                                    |
| a    | 地址                                      |
| c    | 字符                                      |
| s    | 字符串                                    |
| z    | 0结尾的字符串                             |
| i    | 显示为机器语言(仅在显示内存的x命令中可用) |

[例]

```shell
(gdb) p/x $eax
(gdb) x/i $pc  汇编指令
```

一般使用x命令的时候,格式为x/NFU ADDR

此处的ADDR为希望显示的地址,N为重复此时

F为前面讲过的格式(x,d,u,o,t,a,c,f,s,i)

U代表的单位:


| 单位 | 说明 |
| :--- | :--- |
| b    | 字节 |
| h    | 半字 |
| w    | 字   |
| g    | 双字 |

[例]

```shell
(gdb) x/10i $pc
```

反汇编命令 disassemble 简写为disas

格式:

① disassemble

② disassemble 程序计数器

③ disassemble 开始地址 结束地址

[例]

```shell
(gdb) disassemble $pc $pc+20
```

### 单步执行

执行源代码中的一行的命令为next(简写为n),执行时如果遇到函数调用想要执行到函数内部,可以使用step(简写为p)

如果要逐条执行汇编指令,可以分别使用nexti和stepi命令

### 继续运行

调试时,可以使用continue(简写为c)命令继续运行程序,直到遇到下一个断点,如果没有遇到断点就会一直运行到结束

格式:

① continue

② continue 次数

指定次数可以忽略断点,例如continue 5则表示5次遇到断点不停止,在第六次遇到断点时才暂停执行.

被调试的程序通常以下几种情况之一:

① 可以正常结束

② 由于某种原因异常结束(发生内核转储,非法访问等)

③ 无法结束(死循环等)

④ 被挂起(停止响应,死锁 ctrl+c等)

### 监视点

在大型软件或大量使用指针的程序中,很难弄清变量在什么地方被改变,要想找到变量在何处被改变,可以使用watch命令

格式:

① watch <表达式>

<表达式>发生变化时暂停运行.

此处<表达式>的意思的常量或变量等

格式:

awatch <表达式>

<表达式>被访问,改变时暂停运行.

格式:

rwatch <表达式>

<表达式>被访问时暂停运行.

[例]

```shell
(gdb) awatch short_output
```

要注意的是,设置监视点可能会降低运行速度

### 删除断点和监视点

用delete(简写d)命令删除断点和监视点

格式:

delete <编号>

删除<编号>指示的断点或监视点

[例]

```shell
(gdb) info b
Num Type Disp Enb Address    What
1   breakpoint keep y   0x080484f4 in main at main.c:6
2   breakpoint keep y   0x0804850c in main at main.c:8
(gdb) delete 1  删除编号为1的断点
(gdb) info b
Num Type Disp Enb Address    What
2   breakpoint keep y   0x0804850c in main at main.c:8
```

### 其他断点

硬件断点(hbreak),适用于ROM空间无法修改的内存区域的程序,在有些架构中无法使用

临时断点(tbreak)和临时硬件断点(thbreak),和断点(硬件断点)相同,都会在运行到该处时暂停,不同指出就是临时断点(临时硬件断点)会在此时被删除,所以在只需要停止一次时用起来很方便,遗憾的是没有临时监视点

### 改变变量的值

格式:

set variable <变量名>=<值>

[例]

```shell
(gdb) p options
$7 = 1
(gdb) set variable options = 10
(gdb) p options
$8 = 10
```

### 生成内存转储文件

使用 generate-core-file可将调试中的进程生成内核转储文件

[例]

```shell
(gdb) generate-core-file
Saved corefile core.13163
```

有了内核转储文件和调试对象,以后就能生成转储文件时的运行历史(寄存器值,内存值等).
