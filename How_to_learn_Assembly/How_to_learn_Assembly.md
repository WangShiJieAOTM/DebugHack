## 怎样学习汇编语言

#### 查看反汇编的输出结果

```shell
$ objdump -d --no-show-raw-insn assemble
# --no-show-raw-insn:不显示机器指令
# ...
0000000000001158 <func>:
    1158:       endbr64 
    115c:       push   %rbp
    115d:       mov    %rsp,%rbp
    1160:       movq   $0x0,-0x8(%rbp)# 相当于第八行  unsigned long long val64 = 0;
    1168:       movabs $0xffffeeeeddddcccc,%rax #相当于第九行val64 = 0xffffeeeeddddcccc;
    1172:       mov    %rax,-0x8(%rbp)
    1176:       movl   $0x5555,0x2e94(%rip)        # 4014 <global>  给全局变量负责
    1180:       nop
    1181:       pop    %rbp
    1182:       ret      # 返回函数调用处

0000000000001183 <main>:
    1183:       endbr64 
    1187:       push   %rbp
    1188:       mov    %rsp,%rbp
    118b:       sub    $0x30,%rsp
    118f:       mov    %fs:0x28,%rax
    1198:       mov    %rax,-0x8(%rbp)
    119c:       xor    %eax,%eax
    119e:       movl   $0x0,-0x2c(%rbp)
    11a5:       movabs $0x6f57206f6c6c6548,%rax
    11af:       mov    $0x646c72,%edx
    11b4:       mov    %rax,-0x20(%rbp)
    11b8:       mov    %rdx,-0x18(%rbp)
    11bc:       lea    -0x7a(%rip),%rax        # 1149 <func_op>
    11c3:       mov    %rax,-0x28(%rbp)
    11c7:       movl   $0xabcd,-0x2c(%rbp)     # 用movl指令将0xabcd写入到rbp-0x2c处  相当于23行     i = 0xabcd;
    11ce:       cmpl   $0x1234,-0x2c(%rbp)     # if命令  相当于25行     if (i == 0x1234) {
    11d5:       je     11e4 <main+0x61>        # 如果 "i==0x1234"就不执行"i=0"jump equal到-1  je在CPU寄存器是ZF=1时跳转
    11d7:       movl   $0x0,-0x2c(%rbp)        # 相当于26行         i = 0;
    11de:       jmp    11e4 <main+0x61>        # 继续执行跳到下两行的原因是给每次循环的操作创建起点,do while就直接下去了
    11e0:       addl   $0x1,-0x2c(%rbp)        # 相当于29行         i++;
    11e4:       cmpl   $0x0,-0x2c(%rbp)        # 判断i == 0相当于28行    while (i == 0)
    11e8:       je     11e0 <main+0x5d>        # 如果i==0就跳到11e0  je在CPU寄存器是ZF=1时跳转
    11ea:       call   1158 <func>             # 相当于31行     func(); 控制权转给了<func>函数 转到1158
    11ef:       mov    -0x28(%rbp),%rax
    11f3:       call   *%rax                   # 调用函数指针 func_op(),函数指针保存在rax中
    11f5:       mov    %eax,-0x2c(%rbp)        # 得到结果赋值
    11f8:       movl   $0x0,-0x2c(%rbp)        # 相当于for循环的i = 0
    11ff:       jmp    1210 <main+0x8d>        # 开始执行判断条件
    1201:       mov    -0x2c(%rbp),%eax        # i的值赋值给eax
    1204:       movzbl -0x20(%rbp,%rax,1),%eax # 把-0x20(%rbp)的第eax(i)个字节的1个字节赋给eax  -->eax = word[i] 
    1209:       mov    %al,-0x2d(%rbp)         # %al是eax的低8比特,由于char类型的变量只占用一个字节,所以只需要取出eax的低8比特就可以了 这里相当于word = eax === word = word[i]
    120c:       addl   $0x1,-0x2c(%rbp)        # 相当于i++;
    1210:       cmpl   $0xf,-0x2c(%rbp)        # 判断i是否等于15
    1214:       ja     1201 <main+0x7e>        # 如果i>15就跳到1201  ja在CPU寄存器是ZF=0且CF=0时跳转
    1216:       mov    $0x0,%eax               # return 语句返回值为int等四字节以下的情况,要将返回值放在eax寄存器中
    121b:       mov    -0x8(%rbp),%rdx
    121f:       sub    %fs:0x28,%rdx
    1228:       je     122f <main+0xac>
    122a:       call   1050 <__stack_chk_fail@plt>
    122f:       leave  
    1230:       ret    
# ...
```