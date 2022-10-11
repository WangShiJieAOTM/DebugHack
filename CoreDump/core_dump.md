```shell
#!/usr/bin/env bash

ulimit -c #-c选项表示内存转储文件的大小限制,如果输出为0则表示内存转储无效

ulimit -c unlimited #设置为无上限

ulimit -c 1073741824 #设置为1G
```