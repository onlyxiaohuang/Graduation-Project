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
    }

    in.close();
    
}

__type dis(const std::vector<__type> &x,const std::vector<__type> &y){
    __type ret = 0;
    assert(x.size() == y.size());
    for(int i = 0;i < x.size();i ++){
        ret += x[i] * y[i];
    }
    return ret;
}
