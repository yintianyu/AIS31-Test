// 20190521

#ifndef TESTER_H
#define TESTER_H

#include <vector>

typedef long long TestNumber;

class Tester{
    std::vector<TestNumber> numbers;
    public:
    void generate(int quantity);
    bool test0(int quantity);
};

#endif