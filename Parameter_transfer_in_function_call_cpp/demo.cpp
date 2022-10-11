#include"demo.h"
#include<cstdio>

int main() {
    foo f1, f2;
    printf("f1:%p,f2:%p\n", &f1, &f2);
    f1.func(5, 1);
    f2.func(-4, 2);
    return 0;
}

