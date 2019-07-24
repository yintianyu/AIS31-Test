// 20190521

// 功能：AIS31的测试框架
#include <iostream>
#include <cstring>
#include "Tester.h"

int main(int argc, char* argv[]){
    Tester tester;
    tester.generate_pseudo(10000000);
    tester.convert_48(10000000);
    tester.Tic();
    if(!tester.test0(10000000)){
        std::cout << "test0 failed" << std::endl;
    }
    tester.Toc("Test 0");
    tester.Tic();
    if(!tester.test0_hash(10000000)){
        std::cout << "test0 hash failed" << std::endl;
    }
    tester.Toc("Test 0 Hash");
    return 0;
}
