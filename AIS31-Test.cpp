// 20190521

// 功能：AIS31的测试框架
#include <iostream>
#include "Tester.h"

int main(void){
    Tester tester;
    tester.Tic();
    tester.read_real_from_file(65536);
    tester.Toc("Generate Number");
    tester.Tic();
    std::cout << tester.test0(65536) << std::endl;
    tester.Toc("Test0");
    tester.Tic();
    std::cout << tester.test1(20000) << std::endl;
    tester.Toc("Test1");
    tester.Tic();
    std::cout << tester.test2() << std::endl;
    tester.Toc("Test2");
    tester.Tic();
    return 0;
}
