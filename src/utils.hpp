#pragma once

#include <ctime>
#include <cstdlib>
#include <random>

std::mt19937& random_begin(unsigned int seed);

int random_int(std::mt19937 &rd,int l,int r);//return a random integer with same probability from range [l,r]

