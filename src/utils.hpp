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
#include <memory>
#include <chrono>

//eigen
#include "./libs/eigen/Eigen/SVD"

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

//sift
const int sift_dim = 128, sift_base = 10000, sift_query = 100, sift_learn = 25000, sift_K = 100;

//gist
const int gist_dim = 960, gist_base = 1000000, gist_query = 1000, gist_learn = 500000, gist_K = 100;

//finger
const int finger_r = 20;

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
    std::vector< std::shared_ptr< Node> > tonode;

    //@FINGER
    //the P * dres in the neighborhood
    std::vector< Eigen::VectorXf > Pdres;
    Eigen::VectorXf Pqres;
    __type nowdistoq = 0;

};

struct Graph
{
    Graph(){
    //    Nodes.resize(N);
    }
    ~Graph(){
        for(auto tt:Nodes){
            tt.reset();
        }
    }
    std::vector<std::shared_ptr<Node> > Nodes;
};

std::mt19937 &random_begin(unsigned int seed);

int random_int(std::mt19937 &rd, int l, int r); // return a random integer with same probability from range [l,r]

Graph initialize(int N, int L, int R, int seed, int dim);

__type dis(const std::vector<__type> &x,const std::vector<__type> &y);

std::vector<const Node*> Greedy_Graph_Search(Node* q,Node* p,int efs);

void TimeStart();
double TimeEnd();