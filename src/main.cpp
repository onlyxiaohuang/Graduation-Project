#include "utils.hpp"
#include "hnsw.hpp"

#include <vector>
#include <queue>
#include <map>
#include <iterator>
#include <iostream>

extern std::mt19937& random_begin(unsigned int seed);
extern int random_int(std::mt19937 &rd,int l,int r);
extern Graph initialize(int N,int L,int R,int seed,int dim);
extern void Get_Graph(Graph &G,hnswlib::HierarchicalNSW<__type>* alg);

static int seed = 233;

static Graph G;
extern hnswlib::HierarchicalNSW<__type>* build_graph_HNSW(Graph &G);//build graph by HNSW
extern __type dis(const std::vector<__type> &x,const std::vector<__type> &y);//get dis function

void build_graph_simple(){//build the graph with original nodes,
    for(int i = 0;i < G.Nodes.size();i ++){
        for(int j = i + 1;j < G.Nodes.size();j ++){ 
            G.Nodes[i]->tonode.push_back(G.Nodes[j]);
            G.Nodes[j]->tonode.push_back(G.Nodes[i]);
        }
    }
}

extern std::vector<const Node*> Greedy_Graph_Search(Node* q,Node* p,int efs);

int main(){
    G = initialize(N,L,R,seed,dim);

//    build_graph_simple();
    auto alg = build_graph_HNSW(G);

    if(debug == 1){/*
        std::cout << "The Graph vectors are:" << std::endl;
        for(auto tt:G.Nodes){
            std::cout << "Yes" << std::endl;
            for(auto y:tt.vec){
                std::cout << y << " ";
            }
            std::cout << std::endl;
        }*/
    }    
    Get_Graph(G,alg);

    auto T = Greedy_Graph_Search(G.Nodes[0].get(),G.Nodes[1].get(),5);
    
    std::cout << "The results are:" << std::endl;
    
    for(auto tt:T){
        for(auto y:tt->vec){
            std::cout << y << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}