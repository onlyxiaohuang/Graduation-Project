#include "utils.hpp"
#include "hnsw.hpp"

#include <vector>
#include <queue>
#include <assert.h>
#include <map>
#include <set>
#include <iterator>
#include <iostream>

extern std::mt19937& random_begin(unsigned int seed);
extern int random_int(std::mt19937 &rd,int l,int r);
extern Graph initialize(int N,int L,int R,int seed,int dim);
extern void Get_Graph(Graph &G,hnswlib::HierarchicalNSW<__type>* alg);

static int seed = 233;

static Graph G;
extern hnswlib::HierarchicalNSW<__type>* build_graph_HNSW(Graph &G);//build graph by HNSW

__type dis(const std::vector<__type> &x,const std::vector<__type> &y){
    __type ret = 0;
    assert(x.size() == y.size());
    for(int i = 0;i < x.size();i ++){
        ret += x[i] * y[i];
    }
    return ret;
}


void build_graph_simple(){//build the graph with original nodes,
    for(int i = 0;i < G.Nodes.size();i ++){
        for(int j = i + 1;j < G.Nodes.size();j ++){ 
            G.Nodes[i].tonode.push_back(&G.Nodes[j]);
            G.Nodes[j].tonode.push_back(&G.Nodes[i]);
        }
    }
}



std::vector<const Node*> Greedy_Graph_Search(Node q,Node p,int efs){ //FINGER ALGORITHM-1
    auto cmp = [&q](const Node *a,const Node *b) -> bool{
        return dis(a->vec,q.vec) < dis(b->vec,q.vec);
    };//the increasing order
    auto cmp2 = [&q](const Node *a,const Node *b) -> bool{
        return dis(a->vec,q.vec) > dis(b->vec,q.vec);
    };//the decreasing order
    
    std::set<const Node*, decltype(cmp)> C(cmp);//candidate set
    std::set<const Node*, decltype(cmp2)> T(cmp2);//result set
    std::set<Node*> V;

    T.insert(&p);   C.insert(&p);
    while(!C.empty()){
        auto cur = *C.begin();

        if(debug == 1){
            std::cout << &cur << std::endl;
            std::cout << T.size() << std::endl;
        }
        
        auto first = *T.begin();
        __type dist = dis(first->vec,q.vec);
        
        if(dis(cur->vec,q.vec) > dist){
            std::vector <const Node*> tmp;
            for(auto tt:T){
                tmp.push_back(tt);
            }
            return tmp;
        }
        
        for(auto n:cur->tonode){
            if(V.find(n) != V.end()){
                continue;
            }

            V.insert(n);

            if(dis(n->vec,q.vec) - eps <= dist + eps || T.size() <= efs){
                T.insert(n);
                if(T.size() > efs){
                    T.erase(T.begin());
                }
                dist = dis((*T.begin())->vec,q.vec);
            }
        }
    }

    std::vector <const Node*> tmp;
    for(auto tt:T){
        tmp.push_back(tt);
    }
    return tmp;
}

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

    auto T = Greedy_Graph_Search(G.Nodes[0],G.Nodes[1],5);
    
    std::cout << "The results are:" << std::endl;
    
    for(auto tt:T){
        for(auto y:tt->vec){
            std::cout << y << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}