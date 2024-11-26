#pragma once

#include <set>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <random>
#include <assert.h>
#include <algorithm>

// debug
#ifndef DEBUG
const int debug = 0;
#endif

#ifdef DEBUG
const int debug = 1;
#endif

// characters
const int dim = 30;
const int N = 1000, L = -10000, R = 10000;

typedef float __type;

// eps
const __type eps = 1e-7;

//r_proportion
const __type r_proportion = 0.5;

struct Node
{
    Node() = default;
    // vec.resize(dim);

    Node(std::vector<__type> &tmp)
    {
        std::swap(tmp, vec);
    }
    //the index of the node in Graph
    int index;

    //the vector of the Node
    std::vector<__type> vec;

    //the neighbors of the Node
    std::vector<Node *> tonode;

};

struct Graph
{
    Graph(){
    //    Nodes.resize(N);
    }
    std::vector<Node *> Nodes;
};

std::mt19937 &random_begin(unsigned int seed);

int random_int(std::mt19937 &rd, int l, int r); // return a random integer with same probability from range [l,r]

Graph initialize(int N, int L, int R, int seed, int dim);

__type dis(const std::vector<__type> &x,const std::vector<__type> &y);