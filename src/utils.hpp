#pragma once

#include <ctime>
#include <cstdlib>
#include <random>

//debug
const int debug = 1;

//characters
const int dim = 10;
const int N = 10000,L = -10000,R = 10000;

typedef float __type;

struct Node{
    Node() = default;
        //vec.resize(dim);
    
    Node(std::vector <__type> &tmp){
        std::swap(tmp,vec);
    }

    std::vector <__type> vec;
    std::vector <Node*> tonode;

};

struct Graph{
    std::vector<Node> Nodes;
};



std::mt19937& random_begin(unsigned int seed);

int random_int(std::mt19937 &rd,int l,int r);//return a random integer with same probability from range [l,r]

Graph initialize(int N,int L,int R,int seed,int dim);
