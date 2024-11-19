#include "utils.hpp"

#include <vector>
#include <queue>
#include <assert.h>
#include <map>
#include <set>
#include <iterator>
#include <iostream>



extern std::mt19937& random_begin(unsigned int seed);
extern int random_int(std::mt19937 &rd,int l,int r);

typedef int __type;
const int dim = 10;
static int seed = 233;

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
static Graph G;

__type dis(const std::vector<__type> &x,const std::vector<__type> &y){
    __type ret = 0;
    assert(x.size() == y.size());
    for(int i = 0;i < x.size();i ++){
        ret += x[i] * y[i];
    }
    return ret;
}

void initialize(int N,int L,int R){//N nodes with vector in range[L,R]
    auto rd = random_begin(seed);

    for(int i = 1;i <= N;i ++){
        Node node;
        for (int j = 1; j <= dim;j ++){
            node.vec.push_back(random_int(rd,L,R));
        }
        G.Nodes.push_back(node);
    }
}

void build_graph_simple(){//build the graph with original nodes,
    for(int i = 0;i < G.Nodes.size();i ++){
        for(int j = 1;j < G.Nodes.size();j ++){
            G.Nodes[i].tonode.push_back(&G.Nodes[j]);
            G.Nodes[j].tonode.push_back(&G.Nodes[i]);
        }
    }
}

void build_graph_HNSW(){//build graph by HNSW
    
}

std::vector<Node> Greedy_Graph_Search(Node q,Node p,int efs){ //FINGER ALGORITHM-1
    auto cmp = [&q](const Node *a,const Node *b) -> bool{
        return dis(a->vec,q.vec) < dis(b->vec,q.vec);
    };
    std::set<const Node*, decltype(cmp)> T(cmp);
    std::set<Node*> V;

    T.insert(&p);
    while(!T.empty()){
        auto cur = *T.begin();
        auto last = *T.rbegin();
        __type dist = dis(last->vec,q.vec);
        if(dis(cur->vec,q.vec) > dist){
            std::vector <Node> tmp;
            for(auto tt:T){
                tmp.push_back(*tt);
            }
            return tmp;
        }
        
        for(auto n:cur->tonode){
            if(V.find(n) != V.end()){
                continue;
            }

            V.insert(n);

            if(dis(n->vec,q.vec) < dist || T.size() < efs){
                T.insert(n);
                if(T.size() > efs){
                    T.erase(prev(T.end()));
                }
            }

            dist = dis(last->vec,q.vec);
        }
    }

    std::vector <Node> tmp;
    for(auto tt:T){
        tmp.push_back(*tt);
    }
    return tmp;
}

int main(){
    initialize(10,-100,100);
    build_graph();

    std::cout << "The Graph vectors are:" << std::endl;
    for(auto tt:G.Nodes){
        for(auto y:tt.vec){
            std::cout << y << " ";
        }
        std::cout << std::endl;
    }
    
    auto T = Greedy_Graph_Search(G.Nodes[0],G.Nodes[1],5);
    
    std::cout << "The results are:" << std::endl;
    
    for(auto tt:T){
        for(auto y:tt.vec){
            std::cout << y << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}