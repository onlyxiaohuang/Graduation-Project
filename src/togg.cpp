#include "togg.hpp"


//TOGG Algorithm-1
NeighborTree OGS_KDT(Node* p,int ddim,std::vector<__type> &g){
    
    assert(ddim < p -> vec.size());    
    assert(ddim < g.size());

    NeighborTree T;
    T.dim = ddim;
    for(auto tt:p->tonode){
        if( tt->vec[ddim] - p->vec[ddim] <= -g[ddim] ){
            T.left.push_back(tt.get());
        }
        else if( tt->vec[ddim] - p->vec[ddim] >= g[ddim] ){
            T.right.push_back(tt.get());
        }
        else{
            T.common.push_back(tt.get());
        }
    }
    

    return T;
}

//From TOGG Algorithm-2
int get_div_dim(Graph &G,Node * p,__type r_proportion){
    int dim_sz = p -> vec.size();
    __type range_size = *std::max_element(p -> vec.begin() , p -> vec.end()) - *std::min_element(p -> vec.begin(), p -> vec.end());

    int now_div_dim = 0, dim_diff = G.Nodes.size();
    for (int i = 0; i < dim_sz; i ++){
        int lsize = 0,rsize = 0;
        for(auto tt:G.Nodes){
            if( (tt -> vec[i]) - (p ->vec[i]) < -r_proportion * range_size ){
                lsize ++;
            }
            else if( (tt -> vec[i]) - (p ->vec[i]) > r_proportion * range_size ){
                rsize ++;
            }
        }

        if( abs(lsize - rsize) < dim_diff ){
            now_div_dim = i;
            dim_diff = abs(lsize - rsize);
        }
    }

    return now_div_dim;
}

//TOGG Algorithm-2
std::vector<Node *> OGS_KDT_Routing(Graph &G,Node *p,Node *q,int l){

    int i = 0,ddim = get_div_dim(G,p,r_proportion);

    std::vector<__type> g;
    for(int i = 0;i < p -> vec.size();i ++){
        g.push_back( r_proportion * (*std::max_element(p -> vec.begin() , p -> vec.end()) - *std::min_element(p -> vec.begin(), p -> vec.end())));
    }


    auto cmp = [&q](const Node *a,const Node *b) -> bool{
        return dis(a->vec,q->vec) < dis(b->vec,q->vec);
    };//the increasing order

    std::set<const Node*, decltype(cmp)> C(cmp),Visited(cmp);//candidate set
    C.insert(p);
    //std::unordered_set <Node*> Visited;//visited set

    //
    auto it = C.begin();
    while(it != C.end()){
        
        auto tt = *it;
        if(Visited.find(*it) != Visited.end()){
            it++;
            continue;
        }
        Visited.insert(tt);
//        std::cout << tt->index << std::endl;
//        if()

        int ddim = get_div_dim(G,(Node *)tt,r_proportion);
        auto T = OGS_KDT((Node *)tt,ddim,g);

        std::vector<Node *> Z;
        for(auto tt2:T.common){
            Z.push_back(tt2);
        }
        if(q -> vec[ddim] - tt -> vec[ddim] <= -g[ddim]){
            for(auto tt2:T.left){
                Z.push_back(tt2);
            }
        }
        else if(q -> vec[ddim] - tt -> vec[ddim] >= g[ddim]) {
            for(auto tt2:T.right){
                Z.push_back(tt2);
            }
        }
        for(auto tt2:Z){
            for(auto nn:tt2->tonode){
                C.insert(nn.get());
            }
        }

        //Delete element
        auto it2 = C.begin();
        int cnt = 0;
        for(;it2 != C.end();){
            cnt ++;
            if(cnt > l){
                it2 = C.erase(it2);
            }else{
                it2 ++;
            }
        }
        

        it = C.begin();
    }

    std::vector<Node *> ret;
    for(auto tt:C){
        ret.push_back((Node *)tt);
    }
//    std::cout << "finished" << std::endl;
    return ret;
}
