#include "utils.hpp"

#include <iostream>

extern std::mt19937& random_begin(unsigned int seed);

extern int random_int(std::mt19937 &rd,int l,int r);

void test_rand(){
    int N = 20,M = 30;

    const int L = -100 ,R = 100;
    
    

    auto rd = random_begin(233);
    //std::cout << "OK" << std::endl;

    for(int i = 1;i <= N ;i ++){
    //    std::cout << "OK" << std::endl;
        for (int j = 1;j <= M;j ++){
            std::cout << random_int(rd,L,R) << " ";
        }
        std::cout << std::endl;
    }
}

int main(){
    test_rand();
    return 0;
}