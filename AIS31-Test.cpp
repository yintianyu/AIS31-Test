// 20190521

// 功能：AIS31的测试框架
#include <iostream>
#include "Tester.h"

int main(void){
    Tester tester;
    // tester.Tic();
    // tester.read_real_from_file(65536);
    // tester.convert_48(65536);
    // tester.Toc("Generate Number");
    // tester.Tic();
    // std::cout << tester.test0(65536) << std::endl;
    // tester.Toc("Test0");
    // tester.Tic();
    // std::cout << tester.test1(20000) << std::endl;
    // tester.Toc("Test1");
    // tester.Tic();
    // std::cout << tester.test2() << std::endl;
    // tester.Toc("Test2");
    // tester.Tic();
    // std::cout << tester.test3() << std::endl;
    // tester.Toc("Test3");
    // tester.Tic();
    // std::cout << tester.test4() << std::endl;
    // tester.Toc("Test4");
    // tester.Tic();
    // std::cout << tester.test5(1) << std::endl;
    // tester.Toc("Test5");
    // tester.Tic();
    // std::cout << tester.test6() << std::endl;
    // tester.Toc("Test6");
    // tester.Tic();
    // std::cout << tester.test7() << std::endl;
    // tester.Toc("Test7");
    // tester.Tic();
    // tester.read_real_from_file(81920000, "./data/realrandomcat.dat");
    // tester.Toc("Get from server");
    // tester.Tic();
    // std::cout << tester.test8() << std::endl;
    // tester.Toc("Test8");
    // tester.Tic();
    tester.Tic();
    cout << tester.procedureA(0) << endl;
    // cout << tester.procedureB(0) << endl;
    tester.Toc("Procedure A");

    // Test
    // tester.read_real_from_file(65536);
    // tester.convert_48(65536);
    // int T5;
    // cout << tester.test5(1, T5, 0) << endl;
    return 0;
}
