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
    TestNumber *numbers48;
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
        std::cout << name << " takes time: " << (double)(finish - start) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
    }
    private:
    void get_real_from_server(int quantity);
    void read_real_from_file(int quantity, int biasByte=0, std::string path="./data/realrandomcat.dat");
    void generate_pseudo(int quantity);
    void convert_48(int quantity);
    void read2bool(int length, int b[]);
    bool test0(int quantity=65536);
    bool test1(int length=20000);
    bool test2(int length=20000, float downBound=1.03, float upBound=57.4);
    bool test3();
    bool test4();
    bool test5(int tao, int& T5, int time, int length=10000, float downBound=2326, float upBound=2674);
    bool test6a(int n=100000, float a=0.025);
    bool test6b(int& biasA, int n=100000, float a=0.02);
    bool test7a(int& biasA, int n=100000, float upBound=15.13);
    bool test7b(int& biasA, int n=100000, float upBound=15.13);
    bool test8(int L=8, int Q=2560, int K=256000);
    public:
    bool procedureA(int time=0);
    bool procedureB(int time=0);
    ~Tester();
};

#endif