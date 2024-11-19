#include "utils.hpp"
#include "hnsw.hpp"

#include <iostream>

//TESTING FOR MAKING RAND

extern std::mt19937& random_begin(unsigned int seed);

extern int random_int(std::mt19937 &rd,int l,int r);

void test_rand(){
    std::cout << "Starting testing the rand function" << std::endl;
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
    
    std::cout << "End of testing the rand function" << std::endl;
}

//TESTING FOR HNSW.cpp
extern hnswlib::HierarchicalNSW<__type>* build_graph_HNSW(Graph &G);
extern void delete_HNSW(hnswlib::HierarchicalNSW<__type>* alg_hnsw);
hnswlib::HierarchicalNSW<__type>* alg;
extern Graph initialize(int N,int L,int R,int seed,int dim);
static Graph G;
static int seed = 233;

void test_build_graph(){
    std::cout << "Starting testing the graph function" << std::endl;
    // Query the elements for themselves and measure recall
    //std::cout << alg << std::endl;
    
    /*
    float correct = 0;
    for (int i = 0;i < G.Nodes.size();i ++){

        //std::cout << &G.Nodes[i].vec[0] << std::endl;
        auto result = alg->searchKnn((void*)&G.Nodes[i].vec[0],1);

        hnswlib::labeltype label = result.top().second;
        if(label == i)  correct ++;
    }
    
    float recall = correct / G.Nodes.size();
    std::cout << "Recall: " << recall << std::endl;
    */
    
    G = initialize(N,L,R,seed,dim);
    alg = build_graph_HNSW(G);

    if(debug == 1){
        std::cout << "Start for testing, address of alg_hnsw is "  << std::endl; 
        std::cout << alg << std::endl;
    }


    delete_HNSW(alg);
    std::cout << "End of testing the graph function" << std::endl;
}

int main(){
    test_rand();

    
    test_build_graph();



    return 0;
}