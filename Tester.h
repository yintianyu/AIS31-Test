// 20190521

#ifndef TESTER_H
#define TESTER_H

#include <vector>
#include "QRBG-lib/QRBG.h"

typedef long long TestNumber;

class Tester{
    TestNumber *numbers;
    QRBG rndService;
    public:
    Tester();
    void generate(int quantity);
    bool test0(int quantity);
    bool test2();
    ~Tester();
};

#endif