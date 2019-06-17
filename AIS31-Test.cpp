// 20190521

// 功能：AIS31的测试框架
#include <iostream>
#include <cstring>
#include "Tester.h"

int main(int argc, char* argv[]){
    if(argc == 1){
        std::cout << "Please input Test Procedure" << std::endl;
        return -1;
    }
    Tester tester;
    if(std::strcmp(argv[1], "ProcedureA") == 0){
        bool res = tester.procedureA(0);
        cout << "Procedure A Result: " << res << endl;
    }
    else if(std::strcmp(argv[1], "ProcedureB") == 0){
        bool res = tester.procedureB(0);
        cout << "Procedure B Result: " << res << endl;
    }
    else if(std::strcmp(argv[1], "All") == 0){
        bool res1 = tester.procedureA(0);
        bool res2 = tester.procedureB(0);
        cout << "Procedure A Result: " << res1 << endl;
        cout << "Procedure B Result: " << res2 << endl;
    }
    else{
        cout << "Input Parameter Error" << std::endl;
        return -1;
    }
    return 0;
}
