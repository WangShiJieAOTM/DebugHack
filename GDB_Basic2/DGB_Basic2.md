## GDB的基本用法(二)

### attach到进程

要调试守护进程(daemon process)等已经启动的进程,或是调试陷入死循环而无法返回控制台的进程时,可以使用attach命令

格式:

attach pid

执行这一命令就可以attach到进程ID为pid的进程上.

查看进程ID可以使用ps命令

进程信息可以使用info proc命令来显示

### 条件断点

有一种断点仅在特定条件下中断

格式:

break 断点 if 条件

[例]

```shell
(gdb) break 10 if i == 10
```

格式:

① condition 断点编号

② condition 断点编号 条件

该命令可以给指定的断点添加或删除触发条件,第一个格式删除指定编号断点的触发条件,第二个格式为指定断点添加触发条件

### 反复执行

格式:

ignore 断点编号 次数

在编号指定的断点,监视点或捕获点忽略指定的次数.

continue命令和ignore命令一样,也可以指定次数,达到指定次数前,执行到断点时不暂停,二者的意义是相同的

格式:

① continue 次数

② step 次数

③ next 次数

④ stepi 次数

⑤ nexti 次数

这些格式分别执行指定次数相应命令

格式:

① finish

② until

③ until 地址

finish 命令执行完当前函数后暂停,until命令执行完当前函数等代码块后暂停,如果是循环,则在执行完循环后暂停,常用于跳出循环

### 删除断点和禁用断点

用clear命令删除已定义的断点,如果需要保留定义,只想临时禁用断点的话,可以使用disable命令

将禁用的断点重新启用的话,则可以使用enable命令

格式:

① clear

② clear 函数名

③ clear 行号

④ clear 文件名:行号

⑤ clear 文件名:函数名

⑥ clear [breakpoints] 断点编号

格式:

① disable [breakpoints]

② disable [breakpoints] 断点编号

③ disable display 显示编号

④ disable mem 内存区域

如果不指定断点编号,则禁用所有断点,否则禁用指定的断点,第三种格式禁用display命令定义的自动显示,第四种格式禁用mem命令定义的内存区域

可以省略 breakpoints 关键字

格式:

① enable [breakpoints]

② enable [breakpoints] 断点编号

③ enable [breakpoints] once 断点编号

④ enable [breakpoints] delete 断点编号

⑤ enable display 显示编号

⑥ enable mem 内存区域

这些格式用于启用断点等,once使指定的断点只启用一次,也就是说,程序运行到该断点并暂停后,该断点即被禁用,这与delete 命令中的once不同,后者是在运行暂停后删除断点

### 断点命令

格式:

```
commands 断点编号
    命令
    ...
end
```

程序在指定断点出暂停后，就会自动执行命令

### 常用命令及省略形式（别名）

| 命令 | 简写形式    | 说明                  |
| --- |---------|---------------------|
|常用命令|||
|backtrace| bt      | 显示当前函数调用栈           |
| break | b       | 设置断点                |
|continue| c,cont  | 继续执行                |
|delete| d       | 删除断点                |
|finish| f       | 执行完当前函数后暂停          |
|info breakpoints| i b     | 显示断点信息              |
|next| n       | 执行下一行               |
|print| p       | 打印变量值               |
|run| r       | 运行程序                |
|step| s       | 执行下一行,如果是函数调用,则进入函数 |
|x/格式 地址| x/格式 地址 | 格式化显示内存内容           |
|until| u       | 执行完当前函数等代码块后暂停      |
|其他命令|||
|directory| dir     | 插入目录                |
|diasble| dis     | 禁用断点                |
|dowm| do      | 在当前调用的栈帧中选择要显示的栈帧   |
|edit| e       | 编辑文件或函数             |
|frame| f       | 选择当前要显示的栈帧          |
|forward-search | fo      | 向前搜索                |
|generate-core-file| gcore   | 生成内存转储              |
|help| h       | 显示帮助信息              |
|info| i       | 显示信息                |
|list| l       | 显示源代码               |
|nexti| ni      | 执行下一条机器指令           |
|print-object| po      | 显示目标信息              |
|sharedlibrary| share   | 加载共享库的符号            |
|stepi| si      | 执行下一条机器指令,如果是函数调用,则进入函数 |
