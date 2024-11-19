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

Graph initialize(int N,int L,int R,int seed,int dim){//N nodes with vector in range[L,R]
    auto rd = random_begin(seed);
    
    Graph G;
    for(int i = 1;i <= N;i ++){
        Node node;
        for (int j = 1; j <= dim;j ++){
            node.vec.push_back(random_int(rd,L,R));
        }
        G.Nodes.push_back(node);
    }
    return G;
}