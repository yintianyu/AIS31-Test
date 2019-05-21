// 20190521

#include "Tester.h"
#include <random>
#include <set>
#include <cassert>

void Tester::generate(int quantity){
    std::random_device rd;
    std::mt19937_64 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    numbers.clear();
    numbers.resize(quantity);
    for(int i = 0;i < quantity;i++){
        numbers[i] = gen() & 0xffffffffffff;
    }
}

bool Tester::test0(int quantity){
    std::set<TestNumber> s;
    assert(quantity <= numbers.size());
    for(int i = 0;i < quantity;i++){
        std::pair<std::set<TestNumber>::iterator, bool> p = s.insert(numbers[i]);
        if(!p.second){
            return false;
        }
    }
    return true;
}