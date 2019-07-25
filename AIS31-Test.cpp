// 20190521

// 功能：AIS31的测试框架
#include <iostream>
#include <cstring>
#include "Tester.h"

int main(int argc, char* argv[]){
    Tester tester;
    tester.generate_pseudo(65536);
    tester.convert_48(65536);
    tester.Tic();
    if(!tester.test2()){
        std::cout << "test2 failed" << std::endl;
    }
    tester.Toc("Test 2");
    tester.Tic();
    if(!tester.test2_vector()){
        std::cout << "test2_vector failed" << std::endl;
    }
    tester.Toc("Test 2 vector");
    return 0;
}
