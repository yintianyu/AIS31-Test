// 20190521

// 功能：AIS31的测试框架
#include <iostream>
#include "Tester.h"

int main(void){
    Tester tester;
    tester.generate(65536);
    std::cout << tester.test0(65536) << std::endl;
    std::cout << tester.test2() << std::endl;
    return 0;
}
