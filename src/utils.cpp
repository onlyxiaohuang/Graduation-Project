#include "utils.hpp"

std::mt19937& random_begin(unsigned int seed){
    static std::mt19937 rd(seed);
    return rd;
}

int random_int(std::mt19937 &rd,int l,int r){
    int len = r - l + 1;
    int now = rd() % len + l;
    return now;
}
