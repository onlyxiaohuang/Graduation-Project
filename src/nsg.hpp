#include "utils.hpp"
#include "hnsw.hpp"

//贪心搜索时候的参数L
const int SearchL = 10;
//进行nsg建图时的参数M,
const int NsgM = 10;
//for bitset
const int MAXN = 2000000;

std::vector< std::shared_ptr<Node> > Search_on_Graph(Graph &G,std::shared_ptr<Node> p,std::shared_ptr<Node> q,int l);

void Build_NSG_Graph(Graph &G,hnswlib::HierarchicalNSW<__type>* alg);
