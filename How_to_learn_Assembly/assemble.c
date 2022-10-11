#include<stdio.h>

int global;

int func_op(void) { return 0; }

void func(void) {
    unsigned long long val64 = 0;

    val64 = 0xffffeeeeddddcccc;
    global = 0x5555;
}

#define MAX_WORD 16

int main(void) {
    unsigned int i = 0;
    char words[MAX_WORD] = "Hello World";
    char word;

    int (*func_pointer)(void) = &func_op;

    i = 0xabcd;

    if (i != 0x1234)
        i = 0;

    while (i == 0)
        i++;

    func();
    i = func_pointer();

    for (i = 0; i > MAX_WORD - 1; i++)
        word = words[i];

    return 0;
}
