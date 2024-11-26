#include "hnsw.hpp"

extern const int dim,N,L,R;

const int ef_construction = 200,neighbor_size = 516;
hnswlib::HierarchicalNSW<__type>* alg_hnsw = NULL;

hnswlib::HierarchicalNSW<__type>* build_graph_HNSW(Graph &G){//build graph by HNSW
    // Initing index, and __type here only for "float"
    hnswlib::L2Space space(dim);
    alg_hnsw = new hnswlib::HierarchicalNSW<__type>(&space, N, neighbor_size ,ef_construction);
    


    //Add data to index
    for(int i = 0;i < N;i ++){
        alg_hnsw -> addPoint((void*)&G.Nodes[i]->vec[0],i);
    }        

    if(debug == 1){
        std::cout << "Start for testing, address of alg_hnsw is "  << std::endl; 
        std::cout << alg_hnsw << std::endl;
    }

    // Query the elements for themselves and measure recall
    float correct = 0;
    for (int i = 0; i < N; i++) {
        std::priority_queue<std::pair<__type, hnswlib::labeltype>> result = alg_hnsw->searchKnn((void*)&G.Nodes[i]->vec[0], 1);
        hnswlib::labeltype label = result.top().second;
        if (label == i) correct++;
    }
    float recall = correct / N;
    std::cout << "Recall: " << recall << "\n";
    
    //Serialize index
    std::string hnsw_path = "./output/hnsw.bin";
    alg_hnsw->saveIndex(hnsw_path);
    delete alg_hnsw;


    
    // Deserialize index and check recall
    alg_hnsw = new hnswlib::HierarchicalNSW<__type>(&space, hnsw_path);

    if(debug == 1){
        std::cout << "Start for testing, address of alg_hnsw is "  << std::endl; 
        std::cout << alg_hnsw << std::endl;
    }
    correct = 0;
    for (int i = 0; i < N; i++) {
        std::priority_queue<std::pair<__type, hnswlib::labeltype>> result = alg_hnsw->searchKnn((void*)&G.Nodes[i]->vec[0], 1);
        hnswlib::labeltype label = result.top().second;
        if (label == i) correct++;
    }
    recall = (float)correct / N;
    std::cout << "Recall of deserialized index: " << recall << "\n";
    
    return alg_hnsw;
}

void Get_Graph(Graph &G,hnswlib::HierarchicalNSW<__type>* alg){

    for(int i = 0;i < alg -> cur_element_count;i ++){//枚举每一个点的邻居
        //std::cout << "Considering the Node " << i <<"'s neighbors:" << std::endl;
        for(int level = 0 ;level <= alg -> element_levels_[i];level ++){//枚举的是level
            //std::cout << "Now level = "  << level << "," << std::endl;
            
            hnswlib::linklistsizeint *ll_cur = alg -> get_linklist_at_level(i, level);
            int size = alg -> getListCount(ll_cur);
            hnswlib::tableint *data = (hnswlib::tableint *) (ll_cur + 1);
            
            std::vector <Node *> tonode;

            for(int j = 0;j < size;j ++){
                //std::cout << "The " << j << "th neighbor is " << data[j] << std::endl;
                tonode.push_back(G.Nodes[data[j]]);
            }
            G.Nodes[i]->tonode.swap(tonode);
        }
    }

}

void delete_HNSW(hnswlib::HierarchicalNSW<__type>* alg_hnsw){
    delete alg_hnsw;
}
