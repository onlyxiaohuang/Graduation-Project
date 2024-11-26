#include "TOGG.hpp"


//TOGG Algorithm-1
NeighborTree OGS_KDT(Node *p,int ddim,std::vector<__type> &g){
    assert(ddim < p -> vec.size());    
    assert(ddim < g.size());

    NeighborTree T;
    T.dim = ddim;
    for(auto tt:p->tonode){
        if( tt->vec[ddim] - p->vec[ddim] <= -g[ddim] ){
            T.left.push_back(tt);
        }
        else if( tt->vec[ddim] - p->vec[ddim] >= g[ddim] ){
            T.right.push_back(tt);
        }
        else{
            T.common.push_back(tt);
        }
    }
    

    return T;
}

//From TOGG Algorithm-2
int get_div_dim(Graph &G,Node *p,__type r_proportion){
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
std::vector<Node *> OGS_KDT_Routing(Graph &G,Node *p,Node *q,int L){

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

    for(auto tt:C){
        Visited.insert(tt);
        
        int ddim = get_div_dim(G,(Node *)tt,r_proportion);
        auto T = OGS_KDT((Node *)tt,ddim,g);
    }


}
