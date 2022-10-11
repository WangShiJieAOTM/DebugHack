#ifndef DEBUGHACK_DEMO_H
#define DEBUGHACK_DEMO_H

#endif //DEBUGHACK_DEMO_H

class foo {
    int a;
    int b;
public:
    void func(int x, int y);
};

void foo::func(int x, int y) {
    a = x;
    b = y + 2;
}
