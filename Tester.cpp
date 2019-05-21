// 20190521


#include "Tester.h"
#include "QRBG-lib/QRBG.h"
#include <random>
#include <set>
#include <cassert>
#include <map>

Tester::Tester():numbers(NULL){
    rndService.defineServer("random.irb.hr", 1227);
	rndService.defineUser("username", "password");
}

void Tester::generate(int quantity){
    // std::random_device rd;
    // std::mt19937_64 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    // numbers.clear();
    // numbers.resize(quantity);
    // for(int i = 0;i < quantity;i++){
    //     numbers[i] = gen() & 0xffffffffffff;
    // }
    if(numbers != NULL){
        delete[] numbers;
        numbers = NULL;
    }
    numbers = new TestNumber[quantity];
    rndService.getInt64s(numbers, quantity);
}

bool Tester::test0(int quantity){
    std::set<TestNumber> s;
    // assert(quantity <= numbers.size());
    for(int i = 0;i < quantity;i++){
        std::pair<std::set<TestNumber>::iterator, bool> p = s.insert(numbers[i]);
        if(!p.second){
            return false;
        }
    }
    return true;
}

bool Tester::test2() {
    std::map <char, int> m;
    int squareAcc = 0;
    for (int i = 0; i < 5000; i++) {
        char idx = numbers[i] & 0xf;
        ++m[idx];
    }
    for (const auto &e : m) {
        squareAcc += e.first * e.first * e.second;
Tester::~Tester(){
    if(numbers != NULL){
        delete[] numbers;
        numbers = NULL;
    }
}