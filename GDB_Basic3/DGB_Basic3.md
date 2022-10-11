## GDB的基本用法(二)

### 值的历史

通过print命令显示过的值会记录在内部的值历史中,这些值可以在其他表达式中使用

```
(gdb) p argc
$1 = (int *) 0xbff926e00
(gdb) p *argc
$2 = 1
```

最后的值可以用$访问

```
(gdb) p $
$3 = 1
```

用 show value命令可以查看历史中的最后10个值

```
(gdb) show value
$1 = (int *) 0xbff926e00
$2 = 1
$3 = 1
``` 

值历史的访问变量和说明:

|变量| 说明         |
|:---|:-----------|
|$| 值历史的最后一个值  |
|$n| 值历史中的第n个值  |
|$$| 值历史的倒数第二个值 |
|$$n| 值历史中的倒数第n个值 |
|$_|x命令显示过的最后的地址|
|$__|x命令显示过的倒数第二个地址|
|$_exitcode|调试中的程序的返回代码|
|$bpnum|最后一个断点的编号|

### 变量

可以随意定义变量,变量以$开头,由英文字母和数字组成

```
(gdb) set $i = 0
(gdb) p $i
$1 = 0
```

### 命令历史

可以用history命令查看命令历史

默认命令历史文件位于 ./.gdb_history

```
(gdb) show history
```

```
set history filename
show history filename
```

可将命令历史保存到文件中,可通过环境变量GDBHISTFILE改变默认文件名

```
set history save
show history save
```

启用命令历史保存到文件和恢复的功能

```
set history size
show history size
```

设置保存到命令历史中的命令数量,默认值为256

### 初始化文件(.gdbinit)

Linux 环境下的初始化文件为.gdbinit,如果存在.gdbinit文件,GDB就会在启动之前将其作为命令文件运行,
初始化文件和命令文件的运行顺序如下:

① $HOME/.gdbinit

② 运行命令行选项

③ ./.gdbinit

④ 通过-x选项给出的命令文件

初始化文件的语法和命令文件的语法相同,都由gdb命令组成

### 命令文件

自定义命令
格式:

```
define 命令名
    命令
    ...
end
```

给自定义命令添加说明 格式:

```
document 命令名
    说明
end
```

查看定义的命令格式:

```
help 命令名
```

除了初始化文件,还可以把各种设置写在文件中,在运行调试器的时候读取这些文件

格式:

```
source 文件名
```
