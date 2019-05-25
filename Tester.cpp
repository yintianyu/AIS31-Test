// 20190521


#include "Tester.h"
#include "QRBG-lib/QRBG.h"
#include <random>
#include <set>
#include <cassert>
#include <map>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <thread>
#include <functional>

#define THREADNUMBER (8)

Tester::Tester():numbers(NULL), numbers48(NULL){
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

void Tester::read_real_from_file(int quantity, int biasByte, std::string path){
    if(numbers != NULL){
        delete[] numbers;
        numbers = NULL;
    }
    numbers = new TestNumber[quantity];
    FILE *fp = fopen(path.c_str(), "rb");
    if(fp != NULL){
        fseek(fp, biasByte, SEEK_SET);
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

void Tester::convert_48(int quantity){
    if(numbers48 != NULL){
        delete[] numbers48;
        numbers48 = NULL;
    }
    numbers48 = new TestNumber[quantity];
    for(int i = 0;i < quantity;i++){
        numbers48[i] = numbers[i] & 0xffffffffffff;
    }
}


bool Tester::test0(int quantity){
    std::sort(numbers48, numbers48 + quantity);
    for(int i=0; i<quantity-1; i++)
	{
		if(numbers48[i] == numbers48[i+1]){
            std::cout << "T0 Failed" << std::endl;
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
    if(count > 9654 && count < 10346){
        return true;
    }
    std::cout << "T1 value error " << count << " expected 9654-10346" << std::endl;
    return count > 9654 && count < 10346;
}

bool Tester::test2(int length, float downBound, float upBound) {
    std::map <char, int> m;
    int squareAcc = 0;
    int quantity = length / (sizeof(TestNumber) * 8);
    for (int i = 0; i < quantity; i++) {
        for (int j = 0; j < sizeof(TestNumber) * 2; ++j) {
            char idx = (numbers[i] >> (j * 4)) & 0xf;
            ++m[idx];
        }
    }
    for (int i = 0; i < (length % (sizeof(TestNumber) * 8)) / 4; ++i) {
        char idx = (numbers[quantity] >> (4 * i)) & 0xf;
        ++m[idx];
    }
    for (const auto &e : m) {
        squareAcc += e.second * e.second;
    }
    float T2 = squareAcc * 64.0 / length - length / 4;
    if (T2 > downBound && T2 < upBound) {
        return true;
    }
    std::cout << "T2 Wrong value " << T2 << ", out of range " << downBound << " ~ " << upBound << std::endl;
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
        if(Tp0[i] < lowerbound[i] || Tp0[i] > upperbound[i]){
            std::cout << "T3 Tp0[" << i << "] error, got " << Tp0[i] << " expected " << lowerbound[i] <<\
            "-" << upperbound[i] << std::endl;
            result = false;
        }
        if(Tp1[i] < lowerbound[i] || Tp1[i] > upperbound[i]){
            std::cout << "T3 Tp1[" << i << "] error, got " << Tp1[i] << " expected " << lowerbound[i] <<\
            "-" << upperbound[i] << std::endl;
            result = false;
        }
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
                std::cout << "T4 error" << std::endl;
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

bool Tester::test5(int tao, int& T5, int time, int length, float downBound, float upBound) {
    // time = 0: b1 ~ b10000
    // time = 1: b10001 ~ b20000
    std::vector<bool> b(length);
    int quantity = length / (sizeof(TestNumber) * 8);
    int quantity2 = (2 * length ) / (sizeof(TestNumber) * 8);
    T5 = 0;
    if (time == 0) {
        int idx = 0;
        for (int i = 0; i < quantity ; ++i) {
            for (int j = 0; j < sizeof(TestNumber) * 8; ++j) {
                b[idx++] = (numbers[i] >> j) & 1;
            }
        }
        for (int i = 0; i < length % (sizeof(TestNumber) * 8); ++i) {
            b[idx++] = (numbers[quantity] >> i) & 1;
        }
    } else {
        int idx = 0;
        for (int i = length % (sizeof(TestNumber) * 8); i < sizeof(TestNumber) * 8; ++i) {
            b[idx++] = (numbers[quantity] >> i) & 1;
        }
        for (int i = quantity + 1; i < quantity2; ++i) {
            for (int j = 0; j < sizeof(TestNumber) * 8; ++j) {
                b[idx++] = (numbers[i] >> j) & 1;
            }
        }
        for (int i = 0; i < (2 * length) % (sizeof(TestNumber) * 8); ++i) {
            b[idx++] = (numbers[quantity2] >> i) & 1;
        }
    }
    for (int i = 0; i < length / 2; ++i) {
        T5 += b[i] ^ b[i + tao];
    }
    if (T5 > downBound && T5 < upBound) {
        return true;
    }
    std::cout << "T5 Wrong value " << T5 << ", out of range " << downBound << " ~ " << upBound << std::endl;
    return false;
}

// assume k = 1
bool Tester::test6a(int n, float a) {
    // std::vector <bool> b(n);
    int T6 = 0;
    int quantity = n / (sizeof(TestNumber) * 8);
    // int idx = 0;
    for (int i = 0; i < quantity; ++i) {
        for (int j = 0; j < (sizeof(TestNumber) * 8); ++j) {
            T6 += (numbers[i] >> j) & 1;
        }
    }
    for (int i = 0; i < n % (sizeof(TestNumber) * 8); ++i) {
        T6 += (numbers[quantity] >> i) & 1;
    }
    // for (int i = 0; i < n; ++i) {
    //     T6 += b[i];
    // }
    float testVal = (double) T6 / n - 0.5;
    if (testVal > -a && testVal < a) {
        return true;
    }
    std::cout << "T6a Wrong value " << testVal + 0.5 << ", out of range " << 0.5 - a << " ~ " << 0.5 + a << std::endl;
    return false;
}

bool Tester::test6b(int& biasA, int n, float a) {
    bool full[] = {0, 0};
    int count[] = {0, 0};
    int onesCnt[] = {0, 0};
    int zerosCnt[] = {0, 0};
    int idx;
    int quantity = n / (sizeof(TestNumber) * 8);
    double T6;
    while (full[0] + full[1] < 2) {
        read_real_from_file(quantity + 1, biasA);
        for (int i = 0; i < quantity; ++i) {
            for (int j = 0; j < sizeof(TestNumber) * 4; ++j) {
                idx = (numbers[i] >> (2*j)) & 1;
                if (full[idx] == 0) {
                    ++count[idx];
                    if (count[idx] == n) { full[idx] = 1; }
                    onesCnt[idx] += (numbers[i] >> (2*j + 1)) & 1;
                }
            }
        }
        for (int i = 0; i < n % (sizeof(TestNumber) * 8); ++i) {
            idx = (numbers[quantity] >> (2*i)) & 1;
            if (full[idx] == 0) {
                ++count[idx];
                if (count[idx] == n) { full[idx] = 1; }
                onesCnt[idx] += (numbers[quantity] >> (2*i + 1)) & 1;
            }
        }
        biasA += quantity * 8;
    }
    for (int i = 0; i < 2; ++i) { zerosCnt[i] = n - onesCnt[i]; }
    T6 = abs((double) onesCnt[0] / n + (double) zerosCnt[1] / n - 1);
    if (T6 < a) {
        return true;
    } 
    std::cout << "T6b Wrong value " << T6 << ", out of range " << "" << " ~ " << a << std::endl;
    return false;
}

bool Tester::test7a(int& biasA, int n, float upBound) {
    bool full[] = {0, 0, 0, 0};
    int count[] = {0, 0, 0, 0};
    int onesCnt[] = {0, 0, 0, 0};
    int zerosCnt[] = {0, 0, 0, 0};
    int idx;
    int quantity = n / (sizeof(TestNumber) * 8);
    double T7;
    while (full[0] + full[1] + full[2] + full[3] < 4) {
        read_real_from_file(quantity + 1, biasA);
        for (int i = 0; i < quantity; ++i) {
            for (int j = 0; j < sizeof(TestNumber) * 2; ++j) {
                idx =  (numbers[i] >> (4*j)) & 3;
                if (full[idx] == 0) {
                    ++count[idx];
                    if (count[idx] == n) { full[idx] = 1; }
                    onesCnt[idx] += (numbers[i] >> (4*j + 2)) & 1;
                }
            }
        }
        for (int i = 0; i < sizeof(TestNumber) * 2; ++i) {
            idx = (numbers[quantity] >> (4*i)) & 3;
            if (full[idx] == 0) {
                ++count[idx];
                if (count[idx] == n) { full[idx] = 1; }
                onesCnt[idx] += (numbers[quantity] >> (4*i + 2)) & 1;
            }
        }
        biasA += quantity * 8;
    }
    for (int i = 0; i < 4; ++i) { zerosCnt[i] = n - onesCnt[i]; }
    for (int i = 0; i < 2; ++i) {
        T7 = (double) ((zerosCnt[2 * i] - zerosCnt[2 * i + 1]) * (zerosCnt[2 * i] - zerosCnt[2 * i + 1])) / (zerosCnt[2 * i] + zerosCnt[2 * i + 1]) +
            (double) ((onesCnt[2 * i] - onesCnt[2 * i + 1]) * (onesCnt[2 * i] - onesCnt[2 * i + 1])) / (onesCnt[2 * i] + onesCnt[2 * i + 1]);
        if (T7 > 15.13) { 
            std::cout << "T7a Wrong value " << T7 << ", out of range " << "" << " ~ " << upBound << std::endl;
            return false; 
        }
    }
    return true;
}

bool Tester::test7b(int& biasA, int n, float upBound) {
    bool full[] = {0, 0, 0, 0, 0, 0, 0, 0};
    int count[] = {0, 0, 0, 0, 0, 0, 0, 0};
    int onesCnt[] = {0, 0, 0, 0, 0, 0, 0, 0};
    int zerosCnt[] = {0, 0, 0, 0, 0, 0, 0, 0};
    int idx;
    int quantity = n / (sizeof(TestNumber) * 8);
    double T7;
    while (full[0] + full[1] + full[2] + full[3] + full[4] + full[5] + full[6] + full[7] < 8) {
        read_real_from_file(quantity + 1, biasA);
        for (int i = 0; i < quantity; ++i) {
            for (int j = 0; j < sizeof(TestNumber) * 2; ++j) {
                idx =  (numbers[i] >> (4*j)) & 7;
                if (full[idx] == 0) {
                    ++count[idx];
                    if (count[idx] == n) { full[idx] = 1; }
                    onesCnt[idx] += (numbers[i] >> (4*j + 3)) & 1;
                }
            }
        }
        for (int i = 0; i < sizeof(TestNumber) * 2; ++i) {
            idx = (numbers[quantity] >> (4*i)) & 7;
            if (full[idx] == 0) {
                ++count[idx];
                if (count[idx] == n) { full[idx] = 1; }
                onesCnt[idx] += (numbers[quantity] >> (4*i + 3)) & 1;
            }
        }
        biasA += quantity * 8;
    }
    for (int i = 0; i < 8; ++i) { zerosCnt[i] = n - onesCnt[i]; }
    for (int i = 0; i < 4; ++i) {
        T7 = (double) ((zerosCnt[2 * i] - zerosCnt[2 * i + 1]) * (zerosCnt[2 * i] - zerosCnt[2 * i + 1])) / (zerosCnt[2 * i] + zerosCnt[2 * i + 1]) +
            (double) ((onesCnt[2 * i] - onesCnt[2 * i + 1]) * (onesCnt[2 * i] - onesCnt[2 * i + 1])) / (onesCnt[2 * i] + onesCnt[2 * i + 1]);
        if (T7 > 15.13) { 
            std::cout << "T7b Wrong value " << T7 << ", out of range " << "" << " ~ " << upBound << std::endl;
            return false; 
        }
    }
    return true;
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
    if(entropy > 7.976){
        return true;
    }
    std::cout << "T8 failed, enrtopy got " << entropy << " expeced > 7.976" << std::endl;
    return false;
}

bool Tester::procedureA(int time){
    assert(time == 0 || time == 1);
    read_real_from_file(65536);
    convert_48(65536);
    failerTime = 0;
    if(!test0()){
        failerTime = 1;
    }
    for(int i = 0;i < 257;i++){
        read_real_from_file(20000/sizeof(TestNumber) + 1, i * 20000 / 8 + time * 20000 * 257 / 8);
        if(!test1())
            ++failerTime;
        if(!test2())
            ++failerTime;
        if(!test3())
            ++failerTime;
        if(!test4())
            ++failerTime;
        int T5 = 0;
        maxTau = 0;
        // int max = 0, maxTau = 0;
        // for(int j = 1;j <= 5000;j++){
        //     if(!test5(j, T5, 0)){
        //         ++failerTime;
        //         break;
        //     }
        //     if(abs(T5 - 2500) > max){
        //         max = abs(T5 - 2500);
        //         maxTau = j;
        //     }
        // }
        std::future<bool> futures[THREADNUMBER];

        for(int j = 0;j < THREADNUMBER;j++){
            futures[j] = std::async([this, j](){return procedureA_iteration(j);});
        }
        bool res = true;
        for(int j = 0;j < THREADNUMBER;j++){
            res = futures[j].get();
        }
        // bool res = true;
        // for(int j = 0;j < THREADNUMBER;j++){
        //     res &= procedureA_iteration(j);
        // }
        if(res == false)
            ++failerTime;
        if(!test5(maxTau, T5, 1))
            ++failerTime;
        if(failerTime >= 2){
            return false;
        }
    }
    if(failerTime == 1){
        if(time == 1){
            return false;
        }
        return procedureA(1);
    }
    return true;
}

bool Tester::procedureB(int time) {
    assert(time == 0 || time == 1);
    int biasA = 2000000;
    if (time == 1) { biasA = 4000000; }
    int quantity = 100000 / (sizeof(TestNumber) * 8) + 1;
    read_real_from_file(100000 / (sizeof(TestNumber) * 8) + 1, biasA);
    biasA += quantity * 8;
    failerTime = 0;
    if(!test6a()) {
        failerTime = 1;
    }
    if(!test6b(biasA)) {
        ++failerTime;
    }
    if(!test7a(biasA)) {
        ++failerTime;
    }
    if(!test7b(biasA)) {
        ++failerTime;
    }
    read_real_from_file((2560 + 256000), biasA);
    if(!test8()) {
        ++failerTime;
    }
    if(failerTime >= 2) {
        return false;
    }
    else if(failerTime == 1) {
        if(time == 1){
            return false;
        }
        return procedureB(1);
    }
    return true;
}

bool Tester::procedureA_iteration(int iterTime){
    int T5 = 0;
    int max = 0;
    int maxTau = 0;
    int i;
    for(i = iterTime * 5000 / THREADNUMBER + 1;i <= (iterTime + 1) * 5000 / THREADNUMBER;i++){
        if(!test5(i, T5, 0)){
            std::cout << "test5 at iter " << iterTime << ", Number " << i << " failed" << endl;
            // ++failerTime;
            break;
        }
        if(abs(T5 - 2500) > max){
            max = abs(T5 - 2500);
            maxTau = i;
        }
    }
    tauTex.lock();
    if(this->maxTau < maxTau){
        this->maxTau = maxTau;
    }
    tauTex.unlock();
    return (i == (iterTime + 1) * 5000 / THREADNUMBER + 1);
}

Tester::~Tester() {
    if(numbers != NULL){
        delete[] numbers;
        numbers = NULL;
    }
    if(numbers48 != NULL){
        delete[] numbers48;
        numbers48 = NULL;
    }
}