// 20190521

#ifndef TESTER_H
#define TESTER_H

#include <vector>
#include <iostream>
#include <string>
#include "QRBG-lib/QRBG.h"
#include <ctime>
#include <cstdlib>

typedef long long TestNumber;

class Tester{
    TestNumber *numbers;
    QRBG rndService;
    clock_t start, finish;
    int bitcount(TestNumber n);
    public:
    Tester();
    inline void Tic(){
        start = clock();
    }
    inline void Toc(const std::string &name){
        finish = clock();
        std::cout << name << " takes time: " << (double)(finish - start) / CLOCKS_PER_SEC << "s" << endl;
    }
    void generate(int quantity);
    bool test0(int quantity=65536);
    bool test1(int length=20000);
    bool test2();
    ~Tester();
};

#endif