// 20190521


#include "Tester.h"
#include "QRBG-lib/QRBG.h"
#include <random>
#include <set>
#include <cassert>
#include <map>
#include <iostream>
#include <cmath>
#include <cstring>

Tester::Tester():numbers(NULL){
    rndService.defineServer("random.irb.hr", 1227);
	rndService.defineUser("username", "password");
}

int Tester::bitcount(TestNumber n){
    const unsigned int table[256] = 
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
        std::pair<std::set<TestNumber>::iterator, bool> p = s.insert(numbers[i] & 0xffffffffffff);
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

bool Tester::test2(int length, float downBound, float upBound) {
    std::map <char, int> m;
    int squareAcc = 0;
    for (int i = 0; i < length / 4; i++) {
        char idx = numbers[i] & 0xf;
        ++m[idx];
    }
    for (const auto &e : m) {
        squareAcc += e.second * e.second;
    }
    float T2 = squareAcc * 64.0 / length - length / 4;
    if (T2 > downBound && T2 < upBound) {
        return true;
    }
    std::cout << "Wrong value " << T2 << ", out of range " << downBound << " ~ " << upBound << std::endl;
    return false;
}

bool Tester::test3(){
    const unsigned long long bitmap[64] = {
        0x0000000000000001, 0x0000000000000002, 0x0000000000000004, 0x0000000000000008, 0x0000000000000010, 0x0000000000000020, 0x0000000000000040, 0x0000000000000080, 
        0x0000000000000100, 0x0000000000000200, 0x0000000000000400, 0x0000000000000800, 0x0000000000001000, 0x0000000000002000, 0x0000000000004000, 0x0000000000008000, 
        0x0000000000010000, 0x0000000000020000, 0x0000000000040000, 0x0000000000080000, 0x0000000000100000, 0x0000000000200000, 0x0000000000400000, 0x0000000000800000, 
        0x0000000001000000, 0x0000000002000000, 0x0000000004000000, 0x0000000008000000, 0x0000000010000000, 0x0000000020000000, 0x0000000040000000, 0x0000000080000000, 
        0x0000000100000000, 0x0000000200000000, 0x0000000400000000, 0x0000000800000000, 0x0000001000000000, 0x0000002000000000, 0x0000004000000000, 0x0000008000000000, 
        0x0000010000000000, 0x0000020000000000, 0x0000040000000000, 0x0000080000000000, 0x0000100000000000, 0x0000200000000000, 0x0000400000000000, 0x0000800000000000, 
        0x0001000000000000, 0x0002000000000000, 0x0004000000000000, 0x0008000000000000, 0x0010000000000000, 0x0020000000000000, 0x0040000000000000, 0x0080000000000000, 
        0x0100000000000000, 0x0200000000000000, 0x0400000000000000, 0x0800000000000000, 0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000, 
    };
    const int lowerbound[] = {-1, 2267, 1079, 502, 223, 90, 90};
    const int upperbound[] = {-1, 2733, 1421, 748, 402, 223, 223};
    int Tp0[7] = {0};
    int Tp1[7] = {0};
    int state = (numbers[0] & 0x1) != 0;
    int idx = 0;
    int bit = 0;
    int cnt = 1;
    for(int i = 1;i < 20000;++i){
        int current = (numbers[idx] & bitmap[bit]) != 0;
        if(current == state){
            ++cnt;
        }
        else{
            if(state == 0){
                if(cnt >= 6){
                    ++Tp0[6];
                }
                else{
                    ++Tp0[cnt];
                }
            }
            else{
                if(cnt >= 6){
                    ++Tp1[6];
                }
                else{
                    ++Tp1[cnt];
                }
            }
            state = current;
            cnt = 1;
        }
        if(++bit == 64){
            bit = 0;
            ++idx;
        }
    }
    if(state == 0){
        if(cnt >= 6){
            ++Tp0[6];
        }
        else{
            ++Tp0[cnt];
        }
    }
    else{
        if(cnt >= 6){
            ++Tp1[6];
        }
        else{
            ++Tp1[cnt];
        }
    }
    bool result = true;
    for(int i = 1;i < 7;i++){
        if(Tp0[i] < lowerbound[i] || Tp0[i] > upperbound[i])
            result = false;
        if(Tp1[i] < lowerbound[i] || Tp1[i] > upperbound[i])
            result = false;
    }
    return result;
}

bool Tester::test4(){
    const unsigned long long bitmap[64] = {
        0x0000000000000001, 0x0000000000000002, 0x0000000000000004, 0x0000000000000008, 0x0000000000000010, 0x0000000000000020, 0x0000000000000040, 0x0000000000000080, 
        0x0000000000000100, 0x0000000000000200, 0x0000000000000400, 0x0000000000000800, 0x0000000000001000, 0x0000000000002000, 0x0000000000004000, 0x0000000000008000, 
        0x0000000000010000, 0x0000000000020000, 0x0000000000040000, 0x0000000000080000, 0x0000000000100000, 0x0000000000200000, 0x0000000000400000, 0x0000000000800000, 
        0x0000000001000000, 0x0000000002000000, 0x0000000004000000, 0x0000000008000000, 0x0000000010000000, 0x0000000020000000, 0x0000000040000000, 0x0000000080000000, 
        0x0000000100000000, 0x0000000200000000, 0x0000000400000000, 0x0000000800000000, 0x0000001000000000, 0x0000002000000000, 0x0000004000000000, 0x0000008000000000, 
        0x0000010000000000, 0x0000020000000000, 0x0000040000000000, 0x0000080000000000, 0x0000100000000000, 0x0000200000000000, 0x0000400000000000, 0x0000800000000000, 
        0x0001000000000000, 0x0002000000000000, 0x0004000000000000, 0x0008000000000000, 0x0010000000000000, 0x0020000000000000, 0x0040000000000000, 0x0080000000000000, 
        0x0100000000000000, 0x0200000000000000, 0x0400000000000000, 0x0800000000000000, 0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000, 
    };
    int state = (numbers[0] & 0x1) != 0;
    int idx = 0;
    int bit = 0;
    int cnt = 1;
    for(int i = 1;i < 20000;++i){
        int current = (numbers[idx] & bitmap[bit]) != 0;
        if(current == state){
            if(++cnt >= 34){
                return false;
            }
        }
        else{
            cnt = 1;
        }
        if(++bit == 64){
            bit = 0;
            ++idx;
        }
    }
    return true;
}

bool Tester::test5(int tao, int length, float downBound, float upBound) {
    std::vector<bool> b(length);
    int T5 = 0;
    for (int i = 0; i < length; ++i) {
        b[i] = numbers[i] & 1;
    }
    for (int i = 0; i < length / 2; ++i) {
        T5 += b[i] ^ b[i + tao];
    }
    if (T5 > downBound && T5 < upBound) {
        return true;
    }
    std::cout << "Wrong value " << T5 << ", out of range " << downBound << " ~ " << upBound << std::endl;
    return false;
}

bool Tester::test8(int L, int Q, int K){
    const unsigned long long bitmap[] = {
        0x00000000000000ff, 0x000000000000ff00, 
        0x0000000000ff0000, 0x00000000ff000000,
        0x000000ff00000000, 0x0000ff0000000000,
        0x00ff000000000000, 0xff00000000000000};
    int length = pow(2, L);
    int *lastPosition = new int[length];
    memset(lastPosition, 0xff, sizeof(int) * length);
    double entropy;
    double *accumulator = new double[K + Q + 1];
    int distance;

    accumulator[1] = 0;
    for(int i = 1;i < K + Q;i++)
        accumulator[i+1] = accumulator[i] + 1.0 / i;
    for(int i = 1;i <= K + Q;i++)
        accumulator[i] /= log(2);
    for(int i = 0;i < Q;i++){
        // Assume that L = 8
        int byte = i % L;
        int idx = i / (sizeof(TestNumber) / L);
        unsigned char data = numbers[idx] & bitmap[byte] >> (byte * L);
        lastPosition[data] = i;
    }
    entropy = 0;
    for(int i = Q;i < K + Q;i++){
        // Assume that L = 8
        int byte = i % L;
        int idx = i / (sizeof(TestNumber) / L);
        unsigned char data = numbers[idx] & bitmap[byte] >> (byte * L);
        distance = i - lastPosition[data];
        lastPosition[data] = i;
        entropy += accumulator[distance];
    }
    entropy /= K;

    delete[] lastPosition;
    delete[] accumulator;
    return entropy > 7.976;
}

Tester::~Tester() {
    if(numbers != NULL){
        delete[] numbers;
        numbers = NULL;
    }
}