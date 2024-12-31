#include "nsg.hpp"

//需要已经通过一些建好的knn图，这里使用hnsw
//extern hnswlib::HierarchicalNSW<__type>* build_graph_HNSW(Graph &G, int N,int ef_construction,int neighbor_size);//build graph by HNSW

//贪心搜索（算法1），但是返回的是路径上的点集合ret，而不是前L个点集合S
std::vector< std::shared_ptr<Node> > Search_on_Graph(Graph &G,std::shared_ptr<Node> p,std::shared_ptr<Node> q,int l){
    int i = 0;
    std::vector< std::shared_ptr<Node> > S;
    std::vector< std::shared_ptr<Node> > ret;
    S.emplace_back(p);
    std::bitset<MAXN> check;

    //need C++14 模板化lambda
    auto cmp = [&q](auto x, auto y){
        return dis(x -> vec,q -> vec) < dis(y -> vec,q -> vec);
    };

    while(i < l){
        while(check[S[i] -> index]){
            i ++;
            if(i >= l)  break;
        }
        if(i >= l)  break;
        std::shared_ptr<Node> tt(S[i]);
        check[tt -> index] = true;
//        check.insert(tt.get());

        ret.push_back(tt);

        for(auto nn : tt -> tonode){
            S.push_back(tt);
        }

        std::sort(S.begin(),S.end(),cmp);
        auto ed = std::unique(S.begin(),S.end());
        S.erase(ed,S.end());
        if(S.size() > l){
            S.resize(l);
        }
    }
    sort(ret.begin(),ret.end(),cmp);
    return ret;
}


//还没有测试，需要测试

void Build_NSG_Graph(Graph &G,hnswlib::HierarchicalNSW<__type>* alg){
    //首先需要建图，这里采用HNSW建图
    
    Graph NewG;
    int dim = G.Nodes[0]->vec.size();
    unsigned N = G.Nodes.size();
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
    //candidate的l似乎不能乱取,贪心搜索返回的集合似乎只是访问节点的子集
    //使用L2距离

    
    //vector p minus vector q
    auto L2dis = [](std::shared_ptr<Node> p,std::shared_ptr<Node> q){
        __type L2ans = 0;
        for(int i = 0 ;i < p -> vec.size();i ++){
            L2ans += ((p -> vec[i]) - (q -> vec[i])) * ((p -> vec[i]) - (q -> vec[i]));
        }
        return L2ans;
    };
    
    std::vector<std::vector<std::shared_ptr<Node> > >Rs;
    //unsigned N = G.Nodes.size();
    Rs.resize(N);

    for(int i = 0 ;i < N; i++){
        std::shared_ptr<Node> v = G.Nodes[i];
        auto Candidate = Search_on_Graph(G,Navigating_Node,v,SearchL);

        std::vector<std::shared_ptr<Node> > R;
        //if Candidate is not empty
        auto p0 = Candidate[0];    
        Candidate.erase(Candidate.begin());
        R.push_back(p0);

        //from far to close
        std::reverse(Candidate.begin(),Candidate.end());
        while(!Candidate.empty() && R.size() < NsgM){
            auto p = *Candidate.rbegin();
            Candidate.erase(prev(Candidate.end()));

            bool check_insert = true;
            for(auto r: R){
                //if edge pv conflicts with edge pr then
                //也就是说，vp如果不大于pr
                if(L2dis(v,p) + eps <= L2dis(p,r) - eps){
                    break;
                }

                check_insert = false;
            }
            if(check_insert == true){
                R.push_back(p);
            }

        }
        Rs[i].swap(R);
    }

    //第四步,更新新图并且进行dfs使得图联通

    for(auto nodes:G.Nodes){
        nodes -> tonode.swap(Rs[nodes -> index]);
    }

    
    std::bitset<MAXN> check;
    
    //dfs函数
    std::function<void(std::shared_ptr<Node> x)>  dfs = [&check,dfs](std::shared_ptr<Node> x){
        check.set(x -> index,1);
        for(auto y:x -> tonode){
            if(check[y -> index] == false){
                dfs(y);
            }
        }
    };
    
    dfs(Navigating_Node);

    for(int i = 0; i < N;i ++){
        if(check[i] == false){
            auto now = G.Nodes[i];
            int index = Navigating_Node -> index;
            for(int j = 0; j < N ;j ++ ){
                if(check[j] == true){
                    if(L2dis(now,G.Nodes[j]) + eps < L2dis(now,G.Nodes[index])){
                        index = j;
                    }
                }
            }
            now -> tonode.push_back(G.Nodes[index]);

        }
    }

}