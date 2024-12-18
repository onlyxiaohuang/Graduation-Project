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
    std::fstream out("./logs/Test.log");
    auto cmp = [&q](const Node *a,const Node *b) -> bool{
        return dis(a->vec,q->vec) < dis(b->vec,q->vec);
    };//the increasing order

    std::set<const Node*, decltype(cmp)> Visited(cmp);

    __type range = dis((*C.rbegin())->vec,q->vec);
    out << "Start testing TOGG." << std::endl;
    int i = 0;
    while(i < l){

        for(i = 0;i < l;i ++){
            if(Visited.find(C[i]) == Visited.end()){
                break;
            }
        }
        if(i >= l)  break;
        Visited.insert(C[i]);
        
        auto tt = C[i];
        out << tt ->index << " " << dis(tt -> vec,q -> vec) << std::endl;
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

        if(C.size() > l){
            C.resize(l);
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
double cosineSimilarity(const VectorXf& a, const VectorXf& b) {
    return a.dot(b) / (a.norm() * b.norm());
}

//FINGER Algorithm-2
// 方法实现
std::tuple<MatrixXf, __type, __type, __type, __type,__type> computeParameters(std::vector <std::shared_ptr <Node> > v, int finger_r) {
    std::vector<VectorXf> D_res;
    std::vector<std::pair<VectorXf, VectorXf>> S;

    // Step 1: 遍历节点 c
    for(auto nownode : v) {
        for (int c = 0; c < nownode -> tonode.size();c ++) {
            for (int d = c + 1; d < nownode -> tonode.size();d ++) {
                VectorXf fir = Map<VectorXf,Unaligned>((__type *)nownode -> tonode[c] -> vec.data(),nownode -> tonode[c] -> vec.size());
                VectorXf sec = Map<VectorXf,Unaligned>((__type *)nownode -> tonode[d] -> vec.data(),nownode -> tonode[d] -> vec.size());
                S.emplace_back(fir,sec);
            }
        }
    }

    //std::cout << "Step 2" << std::endl;
    // Step 2: 计算 D_res
    for (const auto& pair : S) {
        D_res.push_back(pair.first - pair.second);
    }

    //std::cout << "Step 3" << std::endl;
    // Step 3: 计算 SVD
    MatrixXf matrix(D_res[0].size(), D_res.size());
    for (size_t i = 0; i < D_res.size(); ++i) {
        for (size_t j = 0; j < D_res[i].size(); ++j) {
            matrix(j, i) = D_res[i][j];
        }
    }
    
    
    //std::cout << "Step 31" << std::endl;
    JacobiSVD<MatrixXf> svd(matrix, ComputeThinU | ComputeThinV);
    MatrixXf U = svd.matrixU();//U是一个边长为dim的矩阵
    MatrixXf V = svd.matrixV();

    MatrixXf P = U.leftCols(finger_r).transpose();

    //std::cout << "Step 4" << std::endl;
    // Step 4: 计算 X 和 Y
    std::vector<__type> X, Y;
    for (const auto& pair : S) {
        X.push_back(cosineSimilarity(pair.first, pair.second));
        Y.push_back(cosineSimilarity(P * pair.first, P * pair.second));
    }

    //std::cout << "Step 5" << std::endl;
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

    //std::cout << "Step 6" << std::endl;
    // Step 6: 计算误差 epsilon
    __type epsilon = 0.0;
    for (size_t i = 0; i < N; ++i) {
        epsilon += pow((Y[i] - mu_y) * sigma_x / sigma_y + mu_x - X[i], 2);
    }
    epsilon /= N;

    //std::cout << "End" << std::endl;
    //std::cout << P << " " << mu_x << " " << sigma_x <<" " << mu_y << " " << sigma_y << " " << epsilon << std::endl;
    return {P, mu_x, sigma_x, mu_y, sigma_y, epsilon};
}


//precalculate after Graph G is built
//O(dim * n * d) d is the size of neighbors
std::vector<__type> Dis;
void finger_precalculate_1(Graph &G,std::tuple<MatrixXf,__type,__type,__type,__type,__type> res){

    //calculate ||d||^2
    Dis.resize(G.Nodes.size());
    for(auto node:G.Nodes){
        Dis[node->index] = 0;
        for(auto tt:node -> vec){
            Dis[node -> index] += tt * tt;
        }
    }

    //calculate Pdres 

    VectorXf c,d,q,dres,qres;
    for(auto node:G.Nodes){
        c = Map<VectorXf,Unaligned>((__type *)node -> vec.data(),node -> vec.size());
        node -> Pdres.resize(node -> tonode.size());
        for(int i = 0; i < node -> tonode.size();i ++){
            d = Map<VectorXf,Unaligned>((__type *) node->tonode[i]->vec.data(),node->tonode[i]->vec.size());
            
            //get dres
            dres = d - 1.0 * c.dot(d) / c.dot(c) * c;


            node -> Pdres[i] = std::get<0>(res) * d;

        }
    }
    std::cout << "Precalculated over" << std::endl;
}

void finger_precalculate_2(Graph G,std::tuple<MatrixXf,__type,__type,__type,__type,__type> res,Node *Q){
    VectorXf c,d,q,dres,qres;
    q = Map<VectorXf,Unaligned>((__type *)Q -> vec.data(),Q -> vec.size());
    for(auto node:G.Nodes){
        c = Map<VectorXf,Unaligned>((__type *)node -> vec.data(),node -> vec.size());
        
        //get qres
        qres = q - 1.0 * (c.dot(q)) / (c.dot(c)) * c; 
        node -> Pqres = std::get<0>(res) * q;
    }
}

//FINGER Algorithm-3
__type approximate_distance(const Node *Q,const Node *C,const Node *D,std::tuple<MatrixXf,__type,__type,__type,__type,__type> par){
  //  std::cout << "begin" << std::endl;
    
  //  std::cout << Q << " " << C << " " << D << std::endl;
    VectorXf q,c,d;
    q = Map<VectorXf,Unaligned>((__type *)Q -> vec.data(),Q -> vec.size());
    c = Map<VectorXf,Unaligned>((__type *)C -> vec.data(),C -> vec.size());
    d = Map<VectorXf,Unaligned>((__type *)D -> vec.data(),D -> vec.size());

    //std::cout << c << std::endl;

    VectorXf qproj = 1.0 * c.dot(q) / c.dot(c) * c;
    VectorXf qres = q - qproj;
    VectorXf dproj = 1.0 * c.dot(d) / c.dot(c) * c;
    VectorXf dres = d - dproj;

    auto hatt = cosineSimilarity(std::get<0>(par) * qres, std::get<0>(par) * dres);
    auto t = (hatt - std::get<3>(par)) * std::get<2>(par) / std::get<4>(par) + std::get<1>(par) + std::get<5>(par);
  //  std::cout << "end" << std::endl;
    return t;
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
    auto cmp2 = [](const Node *a,const Node *b) -> bool{
        return a -> index < b -> index;
    };//the increasing index

    std::set<const Node*, decltype(cmp)> C(cmp);
    std::set<const Node*, decltype(cmp2)> Visited(cmp2);//candidate set
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
        std::cout << tt->index << std::endl;
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


static int nowtesting = 0;
//TOGG algorithm 5-test1 by using FINGER
//O(n * logn * d^2 * dim)
std::vector<Node *> OGA_routing_test1(Graph &G,std::vector <Node *> C,Node *q,int l,std::tuple<MatrixXf,__type,__type,__type,__type,__type> res){
    std::fstream out("./logs/Test.log",std::ofstream::app);

    out << "Start testing TOGG-FINGER." << std::endl;
    auto cmp = [&q](const Node *a,const Node *b) -> bool{
        return a->index < b->index;
    };//the increasing order

    nowtesting ++;
    //out << "* " << nowtesting << std::endl;

    finger_precalculate_2(G,res,q);
    std::cout << "Finger_Precalculate is over" << std::endl;

    //low rank calculation
    auto finger_dis = [&res](VectorXf &Pqres,VectorXf &Pdres){
        //std::cout << "finger_dis begins. "<< std::endl;
        auto hatt = cosineSimilarity(Pqres,Pdres);
        
        //std::cout << "finger_dis ends."<< std::endl;
        return (hatt - std::get<3>(res)) * std::get<2>(res) / std::get<4>(res) + std::get<1>(res) + std::get<5>(res);
        
    };

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

        
        auto tt = C[i];
        //set nowdistoq
    
        tt -> nowdistoq = dis(tt->vec,q->vec);

        out << "Nownode is " << tt -> index << " " << tt -> nowdistoq << std::endl;
        //std::cout << "Nownode is " << tt -> index << " " << tt -> nowdistoq << std::endl;

        //std::cout << "*" << tt  << std::endl;
        for(int i = 0;i < tt -> tonode.size();i ++){
            auto n = tt -> tonode[i];
   //         std::cout << 1 << std::endl;
   //         std::cout <<"*" << tt  << std::endl;
            __type ndis = finger_dis(tt -> Pqres,tt -> Pdres[i]);
//            std::cout << "*" << n.get() << std::endl;

            if(Visited.find(n.get()) != Visited.end()){
                if( ndis <= range ){
                    C.push_back(n.get());
                    n ->nowdistoq = ndis;
                }
            }
            else{
                //detect covergence path
                auto h = n.get();
                __type hdis = ndis;

                while(true){
                    //std::cout << 2 << std::endl;
                    //std::cout << "*" << h << std::endl;
                    int x = 0;
                    for(int j = 0;j < h -> tonode.size();j ++){
                        auto hn = h -> tonode[j];
                        
                        if( finger_dis (h -> Pqres,h -> Pdres[j]) < finger_dis (h -> Pqres,h -> Pdres[x]) ){
                            x = j;
                        }
                    }
                    if(h -> tonode.size() == 0){
                        break;
                    }
//                    std::cout << 3 << std::endl;
//                    std::cout << x << " " << h -> tonode.size() << std::endl;
//                    std::cout << "*" << h -> tonode[x] << std::endl;
                    if(finger_dis (h -> Pqres,h -> Pdres[x]) + eps < hdis - eps){
                        hdis = finger_dis (h -> Pqres,h -> Pdres[x]);
                        h = h -> tonode[x].get();
                    }
                    else{
                        break;
                    }

                }
    //            std::cout << 4 << std::endl;
    //            std::cout << "*" << h << std::endl;
                if(hdis <= range){
                    C.push_back(h);
                    h ->nowdistoq = hdis;
                }
            }
        }
        sort(C.begin(),C.end(),[&q](Node *x,Node *y){
            return dis(x -> vec,q -> vec) < dis(y -> vec,q -> vec);//x -> nowdistoq < y -> nowdistoq;
        });
        
        auto ed = std::unique(C.begin(),C.end());
        C.erase(ed,C.end());

        if(C.size() > l){
            C.resize(l);
        }
    }
    
    out.close();

    return C;
}


