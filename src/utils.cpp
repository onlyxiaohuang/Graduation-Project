#include "utils.hpp"

std::mt19937& random_begin(unsigned int seed){
    static std::mt19937 rd(seed);
    return rd;
}

int random_int(std::mt19937 &rd,int l,int r){
    int len = r - l + 1;
    int now = rd() % len + l;
    return now;
}

Graph initialize(int N,int L,int R,int seed,int dim){//N nodes with vector in range[L,R]
    auto rd = random_begin(seed);
    
    Graph G;
    for(int i = 1;i <= N;i ++){
        std::shared_ptr<Node> ptr(new Node());
        ptr -> index = i;
        for (int j = 1; j <= dim;j ++){
            __type tmp = random_int(rd,L,R);
            ptr -> vec.push_back(random_int(rd,L,R));
            //std::cout <<random_int(rd,L,R) << std::endl;
        }
        G.Nodes.push_back(ptr);
    }

    //for(auto tt:G.Nodes){
    //    std::cout << tt->vec[0] << std::endl;
    //}

    return G;
}

//read fvecs data
void load_data(char* filename,float*& data, unsigned& num,unsigned& dim){

    std::cout << "start to load data" << std::endl;
    std::ifstream in(filename,std::ios::binary);
    
    if(!in.is_open()){
        std::cout << "open file error" << std::endl;
        exit( -1 );
    }

    in.read((char*)&dim,sizeof(unsigned));
    in.seekg(0,std::ios::end);

    std::ios::pos_type ss = in.tellg();
    size_t fsize = (size_t)ss;
    num = (unsigned)(fsize / (dim + 1) / 4);
    data = new float[num * dim];

    in.seekg(0,std::ios::beg);
    for(size_t i = 0;i < num ;i ++){
        in.seekg(4,std::ios::cur);
        in.read((char*)data + i * dim * 4,dim * 4);
    }
    /*
    for(size_t i = 0;i < num;i ++){
        std::cout << (float)data[i];
        if(!i){
            std::cout << " ";
            continue;
        }
        if(i % (dim - 1) != 0){
            std::cout << " ";
        }
        else{
            std::cout << std::endl;
        }
    }*/

    in.close();
    std::cout << "load data successfully" << std::endl;
}

__type dis(const std::vector<__type> &x,const std::vector<__type> &y){
    __type ret = 0;
    assert(x.size() == y.size());
    for(int i = 0;i < x.size();i ++){
        ret += (x[i] - y[i]) * (x[i] - y[i]);
    }
    return ret;
}


std::vector<const Node*> Greedy_Graph_Search(Node* q,Node* p,int efs){ //FINGER ALGORITHM-1
    auto cmp = [&q](const Node *a,const Node *b) -> bool{
        return dis(a->vec,q->vec) < dis(b->vec,q->vec);
    };//the increasing order
    auto cmp2 = [&q](const Node *a,const Node *b) -> bool{
        return dis(a->vec,q->vec) > dis(b->vec,q->vec);
    };//the decreasing order
    
    std::set<const Node*, decltype(cmp)> C(cmp);//candidate set
    std::set<const Node*, decltype(cmp2)> T(cmp2);//result set
    std::set<Node*> V;

    T.insert(p);   C.insert(p);
    while(!C.empty()){

        auto it = C.begin();
        C.erase(*it);

        auto cur = *it;
        V.insert((Node *)cur);
//        std::cout << cur << std::endl;
        if(debug == 1){
            std::cout << &cur << std::endl;
            std::cout << T.size() << std::endl;
        }
        
        auto first = *T.begin();
        __type dist = dis(first->vec,q->vec);
        
        if(dis(cur->vec,q->vec) > dist){
            break;
        }
        
        for(auto n:cur->tonode){
            if(V.find(n.get()) != V.end()){
                continue;
            }

            V.insert(n.get());

            if(dis(n->vec,q->vec) <= dist || T.size() <= efs){
                T.insert(n.get());
                C.insert(n.get());
                if(T.size() > efs){
                    T.erase(T.begin());
                }
                dist = dis((*T.begin())->vec,q->vec);
            }
        }
    }

    std::vector <const Node*> tmp;
    for(auto tt:T){
        tmp.push_back(tt);
    }
    return tmp;
}
