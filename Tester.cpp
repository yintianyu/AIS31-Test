// 20190521


#include "Tester.h"
#include "QRBG-lib/QRBG.h"
#include <random>
#include <set>
#include <cassert>
#include <map>
//#include <iostream>

Tester::Tester():numbers(NULL){
    rndService.defineServer("random.irb.hr", 1227);
	rndService.defineUser("username", "password");
}

int Tester::bitcount(TestNumber n){
    unsigned int table[256] = 
    { 
        0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
        4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8, 
    }; 

    return table[n &0xff] +
        table[(n >>8) &0xff] +
        table[(n >>16) &0xff] +
        table[(n >>24) &0xff] +
        table[(n >>32) &0xff] +
        table[(n >>40) &0xff] +
        table[(n >>48) &0xff] +
        table[(n >>56) &0xff];
}

void Tester::get_real_from_server(int quantity){
    if(numbers != NULL){
        delete[] numbers;
        numbers = NULL;
    }
    numbers = new TestNumber[quantity];
    rndService.getInt64s(numbers, quantity);
}

void Tester::read_real_from_file(int quantity, std::string path){
    if(numbers != NULL){
        delete[] numbers;
        numbers = NULL;
    }
    numbers = new TestNumber[quantity];
    FILE *fp = fopen(path.c_str(), "rb");
    if(fp != NULL){
        fread(numbers, sizeof(TestNumber), quantity, fp);
        fclose(fp);
    }
}

void Tester::generate_pseudo(int quantity){
    if(numbers != NULL){
        delete[] numbers;
        numbers = NULL;
    }
    numbers = new TestNumber[quantity]; 
    std::random_device rd;
    std::mt19937_64 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    for(int i = 0;i < quantity;i++){
        numbers[i] = gen();// & 0xffffffffffff;
    }
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

bool Tester::test1(int length){
    int quantity = length / (sizeof(TestNumber) * 8);
    int count = 0;
    for(int i = 0;i < quantity;i++){
        count += bitcount(numbers[i]);
    }
    return count > 9654 && count < 10346;
}

bool Tester::test2() {
    std::map <char, int> m;
    int squareAcc = 0;
    for (int i = 0; i < 5000; i++) {
        char idx = numbers[i] & 0xf;
        ++m[idx];
    }
    for (const auto &e : m) {
        //std::cout << e.first << " " << e.second << std::endl;
        squareAcc += e.second * e.second;
    }
    float T2 = squareAcc * 16.0 / 5000.0 - 5000;
    //std::cout << T2 << std::endl;
    if (T2 > 1.03 && T2 < 57.4) {
        return true;
    }
    return false;
}

Tester::~Tester() {
    if(numbers != NULL){
        delete[] numbers;
        numbers = NULL;
    }
}