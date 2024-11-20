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

void test_Get_Linklists(){
    std::cout << "Starting testing the graph get_linklists function" << std::endl;
    
    G = initialize(N,L,R,seed,dim);
    alg = build_graph_HNSW(G);
    
    for(int i = 0;i < alg -> cur_element_count;i ++){//枚举每一个点的邻居
        std::cout << "Considering the Node " << i <<"'s neighbors:" << std::endl;
        for(int level = 0 ;level <= alg -> element_levels_[i];level ++){//枚举的是level
            std::cout << "Now level = "  << level << "," << std::endl;

            hnswlib::linklistsizeint *ll_cur = alg -> get_linklist_at_level(i, level);
            int size = alg -> getListCount(ll_cur);
            hnswlib::tableint *data = (hnswlib::tableint *) (ll_cur + 1);
            
            for(int j = 0;j < size;j ++){
                std::cout << "The " << j << "th neighbor is " << data[j] << std::endl;
            }
        }
    }

    alg->saveIndex("./output/test.bin");

    delete_HNSW(alg);

    std::cout << "Starting testing the graph get_link_lists function" << std::endl;
}

void Get_Graph(Graph &G,hnswlib::HierarchicalNSW<__type>* alg);
void test_Get_Graph(){
    G = initialize(N,L,R,seed,dim);
    alg = build_graph_HNSW(G);
    Get_Graph(G,alg);

    for(int i = 0; i < G.Nodes.size(); i ++){

        for(int j = 0; j < G.Nodes[i].tonode.size(); j ++){
            std::cout << "Node " << i << " to " << G.Nodes[j].index << std::endl;
        }
    }
    
    delete_HNSW(alg);
}

int main(){
    
    test_rand();
    test_build_graph();
    test_Get_Linklists();
    test_Get_Graph();
    
    return 0;
}