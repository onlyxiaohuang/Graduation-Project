#include "nsg.hpp"

//需要已经通过一些建好的knn图，这里使用hnsw
//extern hnswlib::HierarchicalNSW<__type>* build_graph_HNSW(Graph &G, int N,int ef_construction,int neighbor_size);//build graph by HNSW

std::vector< Node * > Search_on_Graph(Graph &G,std::shared_ptr<Node> p,std::shared_ptr<Node> q,int l){
    int i = 0;
    std::vector< Node * > S;
    S.emplace_back(p);
    std::unordered_set< Node *> check;

    auto cmp = [&q](Node *x, Node *y){
        return dis(x -> vec,q -> vec) < dis(y -> vec,q -> vec);
    };

    while(i < l){
        while(check.find(S[i]) != check.end()){
            i ++;
            if(i >= l)  break;
        }
        if(i >= l)  break;
        std::shared_ptr<Node> tt(S[i]);
        check.insert(tt.get());
        for(auto nn : tt -> tonode){
            S.push_back(tt.get());
        }

        std::sort(S.begin(),S.end(),cmp);
        auto ed = std::unique(S.begin(),S.end());
        S.erase(ed,S.end());
        if(S.size() > l){
            S.resize(l);
        }
    }
    return S;
}

void Build_NSG_Graph(Graph &G,hnswlib::HierarchicalNSW<__type>* alg){
    //首先需要建图，这里采用HNSW建图
    
    Graph NewG;
    int dim = G.Nodes[0]->vec.size();
    int N = G.Nodes.size();
    std::vector<__type> midQ(dim);

    //由于节点保存的是地址，所以似乎不能直接浅拷贝过来，不然会被delete删掉里面的内容
//    NewG.Nodes = G.Nodes;
//    for(int i = 0; i < N;i ++){
//
//    }


    //第二步，计算中心节点 midQ
    for(int i = 0; i < N; i ++){
        std::shared_ptr<Node> NowNodeptr = G.Nodes[i];
        for(int k = 0; k < dim;k ++){
            midQ[k] += NowNodeptr -> vec[k];
        }
    }

    for(int i = 0 ;i < dim; i++){
        midQ[i] /= N;
    }

    auto PriQ = alg -> searchKnn(&midQ,1);
    int index = PriQ.top().second;
    std::shared_ptr<Node> Navigating_Node = G.Nodes[index]; 

    //第三步，对于每个点，找到备选集(路径上的点)以及利用MRNG的选择策略来选择m个点
    //candidate的l似乎不能乱取
    

    for(int i = 0 ;i < N; i++){
        std::shared_ptr<Node> NowNodeptr = G.Nodes[i];
        s


    }


}