## 函数调用时的参数传递方法(i376)

#### i386下的函数调用

i386也像x86_64那样可以将部分参数放在寄存器中进行调用,一般这种调用方式称为 fastcall(快速调用).具体使用哪个寄存器则依赖与具体实现.

GCC在函数中声明添加__attribube__((regparm(3))),即可进行这种调用,这样即可使用eax,edx,ecx三个寄存器进行参数传递.

在Linux内核中可以使用FASTCALL或asmregparm宏来实现该功能.

基本上,i386将所有的参数保存到栈上,但是通过GCC的扩展功能__attribute__((regparm(3))),也可以实现部分参数的寄存器传递.

