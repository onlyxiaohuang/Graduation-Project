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

//TOGG algorithm 5
std::vector<Node *> OGA_routing(Graph &G,std::vector <Node *> C,Node *q,int l){
    auto cmp = [&q](const Node *a,const Node *b) -> bool{
        return dis(a->vec,q->vec) < dis(b->vec,q->vec);
    };//the increasing order

    std::set<const Node*, decltype(cmp)> Visited(cmp);

    __type range = dis((*C.rbegin())->vec,q->vec);

    int i = 0;
    while(i < l){
        for(i = 0;i < l;i ++){
            if(Visited.find(C[i]) == Visited.end()){
                break;
            }
        }
        if(i >= l)  break;
        Visited.insert(C[i]);
        
        auto &tt = C[i];
        for(auto n: tt->tonode){
            if(Visited.find(n.get()) != Visited.end()){
                if( dis(n->vec, q->vec) <= range ){
                    C.push_back(n.get());
                }
            }
            else{
                //detect covergence path
                auto h = n;
                while(true){
                    auto x = h;
                    for(auto hn: h->tonode){
                        if( dis(hn -> vec,q -> vec) < dis( x -> vec , q -> vec) ){
                            x = hn;
                        }
                    }

                    if(dis(x -> vec,q -> vec) < dis(h -> vec,q -> vec)){
                        h = x;
                    }
                    else{
                        break;
                    }

                }
                if(dis(h -> vec,q -> vec) <= range){
                    C.push_back(h.get());
                }
            }
        }
        sort(C.begin(),C.end(),[&q](Node *x,Node *y){
            return dis(x -> vec,q -> vec) < dis(y -> vec,q -> vec);
        });
        
        auto ed = std::unique(C.begin(),C.end());
        C.erase(ed,C.end());

        if(C.size() > L){
            C.resize(L);
        }
    }
    
    return C;
}

//
// THE FIRST TEST:Use finger
//

//FINGER METHOD
//graph G,rank r
//O(n * d^2) d is the size of neighbors
//奇异值分解的复杂度是n^3 + mn^2
std::vector<__type> res (std::vector<__type> &x, std::vector<__type> &y){
    std::vector<__type> ret;
    ret.resize(x.size());
    for(int i = 0;i < x.size();i ++){
        ret[i] = x[i] - y[i];
    }
    return ret;
}

using namespace Eigen;
//n是结果矩阵的大小，也就是p.size() * ( p.size() - 1 ) / 2double cosineSimilarity(const VectorXd& a, const VectorXd& b) {
double cosineSimilarity(const VectorXd& a, const VectorXd& b) {
    return a.dot(b) / (a.norm() * b.norm());
}

//FINGER Algorithm-2
// 方法实现
std::tuple<MatrixXd, __type, __type, __type, __type,__type> computeParameters(
    std::vector <std::shared_ptr <Node> > v, int r) {
    std::vector<VectorXd> D_res;
    std::vector<std::pair<VectorXd, VectorXd>> S;

    // Step 1: 遍历节点 c
    for(auto nownode : v) {
        for (int c = 0; c < nownode -> tonode.size();c ++) {
            for (int d = c + 1; d < nownode -> tonode.size();d ++) {
                S.emplace_back(nownode -> tonode[c] -> vec,nownode -> tonode[d] -> vec);
            }
        }
    }

    // Step 2: 计算 D_res
    for (const auto& pair : S) {
        D_res.push_back(pair.first - pair.second);
    }

    // Step 3: 计算 SVD
    MatrixXd D_res_matrix(D_res.size(), r);
    for (size_t i = 0; i < D_res.size(); ++i) {
        D_res_matrix.row(i) = D_res[i].transpose();
    }
    JacobiSVD<MatrixXd> svd(D_res_matrix, ComputeThinU | ComputeThinV);
    MatrixXd U = svd.matrixU();
    MatrixXd V = svd.matrixV();
    MatrixXd P = U.leftCols(r).transpose();

    // Step 4: 计算 X 和 Y
    std::vector<__type> X, Y;
    for (const auto& pair : S) {
        X.push_back(cosineSimilarity(pair.first, pair.second));
        Y.push_back(cosineSimilarity(P * pair.first, P * pair.second));
    }

    // Step 5: 计算分布参数
    __type mu_x,sigma_x,mu_y,sigma_y;
    size_t N = X.size();
    mu_x = accumulate(X.begin(), X.end(), 0.0) / N;
    sigma_x = sqrt(accumulate(X.begin(), X.end(), 0.0, [mu_x](double sum, double x) {
        return sum + (x - mu_x) * (x - mu_x);
    }) / N);
    mu_y = accumulate(Y.begin(), Y.end(), 0.0) / N;
    sigma_y = sqrt(accumulate(Y.begin(), Y.end(), 0.0, [mu_y](double sum, double y) {
        return sum + (y - mu_y) * (y - mu_y);
    }) / N);

    // Step 6: 计算误差 epsilon
    __type epsilon = 0.0;
    for (size_t i = 0; i < N; ++i) {
        epsilon += pow((Y[i] - mu_y) * sigma_x / sigma_y + mu_x - X[i], 2);
    }
    epsilon /= N;

    return {P, mu_x, sigma_x, mu_y, sigma_y, epsilon};
}

//FINGER Algorithm-3
__type approximate_distance(const Node *Q,const Node *C,std::tuple<MatrixXd,__type,__type,__type,__type,__type> par){
    VectorXd q(Q -> vec),c(C -> vec);
    VectorXd qproj = 1.0 * c.dot(q) / c.dot(c) * c;
    
}



//TOGG Algorithm-2-Test1
std::vector<Node *> OGS_KDT_Routing_test1(Graph &G,Node *p,Node *q,int l){

    __type r_proportion;

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
//TOGG algorithm 5-test1
std::vector<Node *> OGA_routing_test1(Graph &G,std::vector <Node *> C,Node *q,int l){
    auto cmp = [&q](const Node *a,const Node *b) -> bool{
        return dis(a->vec,q->vec) < dis(b->vec,q->vec);
    };//the increasing order

    std::set<const Node*, decltype(cmp)> Visited(cmp);

    __type range = dis((*C.rbegin())->vec,q->vec);

    int i = 0;
    while(i < l){
        for(i = 0;i < l;i ++){
            if(Visited.find(C[i]) == Visited.end()){
                break;
            }
        }
        if(i >= l)  break;
        Visited.insert(C[i]);
        
        auto &tt = C[i];
        for(auto n: tt->tonode){
            if(Visited.find(n.get()) != Visited.end()){
                if( dis(n->vec, q->vec) <= range ){
                    C.push_back(n.get());
                }
            }
            else{
                //detect covergence path
                auto h = n;
                while(true){
                    auto x = h;
                    for(auto hn: h->tonode){
                        if( dis(hn -> vec,q -> vec) < dis( x -> vec , q -> vec) ){
                            x = hn;
                        }
                    }

                    if(dis(x -> vec,q -> vec) < dis(h -> vec,q -> vec)){
                        h = x;
                    }
                    else{
                        break;
                    }

                }
                if(dis(h -> vec,q -> vec) <= range){
                    C.push_back(h.get());
                }
            }
        }
        sort(C.begin(),C.end(),[&q](Node *x,Node *y){
            return dis(x -> vec,q -> vec) < dis(y -> vec,q -> vec);
        });
        
        auto ed = std::unique(C.begin(),C.end());
        C.erase(ed,C.end());

        if(C.size() > L){
            C.resize(L);
        }
    }
    
    return C;
}


