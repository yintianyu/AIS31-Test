// 20190521

#ifndef TESTER_H
#define TESTER_H

#include <vector>
#include "QRBG-lib/QRBG.h"

typedef long long TestNumber;

class Tester{
    TestNumber *numbers;
    QRBG rndService;
    int bitcount(TestNumber n);
    public:
    Tester();
    void generate(int quantity);
    bool test0(int quantity);
    bool test1(int length);
    bool test2();
    ~Tester();
};

#endif