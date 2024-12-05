#include "utils.hpp"
#include "hnsw.hpp"

#include <iostream>

//TESTING FOR MAKING RAND

extern std::mt19937& random_begin(unsigned int seed);

extern int random_int(std::mt19937 &rd,int l,int r);

void test_rand(){
    std::cout << "Starting testing the rand function" << std::endl;
    int N = 20,M = 30;

    const int L = -100 ,R = 100;

    auto rd = random_begin(233);
    //std::cout << "OK" << std::endl;

    for(int i = 1;i <= N ;i ++){
    //    std::cout << "OK" << std::endl;
        for (int j = 1;j <= M;j ++){
            std::cout << random_int(rd,L,R) << " ";
        }
        std::cout << std::endl;
    }
    
    std::cout << "End of testing the rand function" << std::endl;
}

//TESTING FOR HNSW.cpp
hnswlib::HierarchicalNSW<__type>* build_graph_HNSW(Graph &G, int N,int Dim = dim,int ef_construction = 200,int neighbor_size = 516);
extern void delete_HNSW(hnswlib::HierarchicalNSW<__type>* alg_hnsw);
hnswlib::HierarchicalNSW<__type>* alg;
extern Graph initialize(int N,int L,int R,int seed,int dim);
static Graph G;
static int seed = 233;

void test_build_graph(){
    std::cout << "Starting testing the graph function" << std::endl;
    // Query the elements for themselves and measure recall
    //std::cout << alg << std::endl;
    
    /*
    float correct = 0;
    for (int i = 0;i < G.Nodes.size();i ++){

        //std::cout << &G.Nodes[i].vec[0] << std::endl;
        auto result = alg->searchKnn((void*)&G.Nodes[i].vec[0],1);

        hnswlib::labeltype label = result.top().second;
        if(label == i)  correct ++;
    }
    
    float recall = correct / G.Nodes.size();
    std::cout << "Recall: " << recall << std::endl;
    */
    
    G = initialize(N,L,R,seed,dim);
    //for(auto tt:G.Nodes){
    //    std::cout << tt->vec[0] << std::endl;
    //}

    alg = build_graph_HNSW(G,N);

    if(debug == 1){
        std::cout << "Start for testing, address of alg_hnsw is "  << std::endl; 
        std::cout << alg << std::endl;
    }

    delete_HNSW(alg);
    std::cout << "End of testing the graph function" << std::endl;
}

void test_Get_Linklists(){
    std::cout << "Starting testing the graph get_linklists function" << std::endl;
    
    G = initialize(N,L,R,seed,dim);
    alg = build_graph_HNSW(G,N);
    
    for(int i = 0;i < alg -> cur_element_count;i ++){//枚举每一个点的邻居
        std::cout << "Considering the Node " << i <<"'s neighbors:" << std::endl;
        for(int level = 0 ;level <= alg -> element_levels_[i];level ++){//枚举的是level
            std::cout << "Now level = "  << level << "," << std::endl;

            hnswlib::linklistsizeint *ll_cur = alg -> get_linklist_at_level(i, level);
            int size = alg -> getListCount(ll_cur);
            hnswlib::tableint *data = (hnswlib::tableint *) (ll_cur + 1);
            
            for(int j = 0;j < size;j ++){
                std::cout << "The " << j << "th neighbor is " << data[j] << std::endl;
            }
        }
    }

    alg->saveIndex("./output/test.bin");

    delete_HNSW(alg);

    std::cout << "Starting testing the graph get_link_lists function" << std::endl;
}

void Get_Graph(Graph &G,hnswlib::HierarchicalNSW<__type>* alg);
void test_Get_Graph(){
    G = initialize(N,L,R,seed,dim);
    alg = build_graph_HNSW(G,N);
    Get_Graph(G,alg);

    for(int i = 0; i < G.Nodes.size(); i ++){

        for(int j = 0; j < G.Nodes[i]->tonode.size(); j ++){
            std::cout << "Node " << i << " to " << G.Nodes[j]->index << std::endl;
        }
    }
    
    delete_HNSW(alg);
}

//TEST TOGG Algorithm 1 & 2, OGS-KDT
extern std::vector<Node *> OGS_KDT_Routing(Graph &G,Node *p,Node *q,int L);
void test_OGS_KDT_Routing(){
    std::cout << "Starting testing the OGS_KDT_Routing function" << std::endl;
    //build the Graph
    G = initialize(N,L,R,seed,dim);


    alg = build_graph_HNSW(G,N);
    Get_Graph(G,alg);

    //testing the OGS
    std::shared_ptr<Node> p = *G.Nodes.begin(), q = *G.Nodes.rbegin();
    int l = 10;
    auto RetAns = OGS_KDT_Routing(G,p.get(),q.get(),l);
    
//    std::cout << "The result is:" << std::endl;
//    for(auto tt:RetAns){
//        std::cout << tt << " ";
//    }

    int correct = 0,K = 10;//recall@K
    for(int i = 0;i < N;i ++){
        std::cout << "*" << i << std::endl;
        auto result = alg -> searchKnn((void *)&(G.Nodes[i]->vec[0]) ,K);
        
        std::set<int> ans;
        auto testresult = OGS_KDT_Routing(G,p.get(),G.Nodes[i].get(),l);
        for(auto tt:testresult){
            ans.insert(tt -> index);
        }
        while(!result.empty()){
            auto now = result.top(); result.pop();
            if(ans.find(now.second) != ans.end()){
                correct ++;
            }
        }

    }
    float recall = 1.0 * correct / N / K;
    
    std::cout << "The recall is" <<  recall << std::endl;

    std::cout << "End of testing the OGS_KDT_Routing function" << std::endl;
    delete_HNSW(alg);


}

//TEST LOADING DATA
extern void load_data(char* filename,float*& data, unsigned& num,unsigned& dim);

void test_load_data_sift(){
    std::cout << "Start testing the loading data" << std::endl;

    char* filename = "./test/siftsmall/siftsmall_base.fvecs";
    //gist
    __type *data = NULL;
    unsigned int gb = sift_base,dm = sift_dim;
    load_data(filename,data,gb,dm);
    
    int nowindex = 0;
    G.Nodes.resize(gb);
    for(int num = 0; num < gb;num ++){
        G.Nodes[num] = std::make_shared<Node>();
        auto &tt = G.Nodes[num];
        //std::cout << nowindex << std::endl;
        for(int i = 0; i < dm;i ++){
            tt->vec.push_back(data[nowindex ++]);
        }
        tt -> index = num;
        //G.Nodes.push_back(tt);
    }

    delete data;//free the data 一定要删掉
    

    std::cout << "End of testing the loading data" << std::endl;

}

void test_load_data_gist(){
    std::cout << "Start testing the loading data" << std::endl;

    char* filename = "./test/gist/gist_base.fvecs";
    //gist
    __type *data = NULL;
    unsigned int gb = gist_base,dm = gist_dim;
    load_data(filename,data,gb,dm);
    
    int nowindex = 0;
    G.Nodes.resize(gb);
    for(int num = 0; num < gb;num ++){
        G.Nodes[num] = std::make_shared<Node>();
        auto &tt = G.Nodes[num];
        //std::cout << nowindex << std::endl;
        for(int i = 0; i < dm;i ++){
            tt->vec.push_back(data[nowindex ++]);
        }
        tt -> index = num;
        //G.Nodes.push_back(tt);
    }

    delete data;//free the data 一定要删掉
    

    std::cout << "End of testing the loading data" << std::endl;

}

extern std::vector<const Node*> Greedy_Graph_Search(Node* q,Node* p,int efs);
void test_Greedy_Search(int testnum = 10,int K = 10,int ef = 200,int nb = 8){
    std::fstream out("./logs/Greedy_Search.log",std::ofstream::app);
    time_t start,stop,usedtime = 0;

    std::cout << "Start testing the greedy search by using sift" << std::endl;
    test_load_data_sift();
    
    std::cout << "Start getting the HNSW Graph" << std::endl;
    alg = build_graph_HNSW(G,sift_base,sift_dim,ef,nb);
    std::cout << alg << std::endl;

    int correct = 0;

    //for (int i = 0; i < N; i++) {
    //    std::priority_queue<std::pair<__type, hnswlib::labeltype>> result = alg->searchKnn((void*)&(G.Nodes[i]->vec[0]), 1);
    //    hnswlib::labeltype label = result.top().second;
    //    if (label == i) correct++;
    //    std::cout << "label is " << label << std::endl;

        //if(debug == 1){
        //    std::cout << G.Nodes[i]->vec[0] << std::endl;
        //}

//    }

    Get_Graph(G,alg);
    std::cout << alg << std::endl;
    std::cout << "End of getting the HNSW Graph" << std::endl;

    //ask for recall
    //recall@K
    for(int i = 0;i < testnum;i ++){

        std::cout << "*" << i << std::endl;
        std::priority_queue<std::pair<__type, hnswlib::labeltype>> result = alg->searchKnn((void*)&(G.Nodes[i]->vec[0]), K);
        
        std::set<int> ans;

        start = time(NULL);
        auto testresult = Greedy_Graph_Search(G.Nodes[i].get(),G.Nodes[0].get(),K);
        stop = time(NULL);

        std::cout << "testresult:" << std::endl;
        for(auto tt:testresult){
            ans.insert(tt -> index);
            std::cout << tt -> index << " ";
            std::cout << "dis:" << dis(tt -> vec,G.Nodes[i]->vec) << " ";
        }
        std::cout << std::endl;

        std::cout << "Ansresult:" << std::endl;
        int dd = K;
        while(dd--){
            auto now = result.top(); result.pop();
            std::cout << now.second << " ";
            std::cout << "dis:" << dis(G.Nodes[now.second] -> vec,G.Nodes[i]->vec) << " ";
            if(ans.find(now.second) != ans.end()){
                correct ++;
            }
        }
        std::cout << std::endl;

    //    std::cout << "Now Node:" << std::endl;
    //    std::cout << i <<" " <<  dis(G.Nodes[i] -> vec,G.Nodes[i] -> vec) << std::endl;

    }
    float recall = 1.0 * correct / testnum / K;
    std::cout << "Recall@" << K << " is " << recall << "." << std::endl;

    out << "Nowtime is " << stop << "." << std::endl;
    out << "Use sift_10000, neighborsize = " << nb << " , ef_construction = " << ef << "." << std::endl;

    out << "testnum = " << testnum << ", K =" << K << "." << std::endl;
    out << "" << std::endl;
    out << "Recall@K = " << recall << ". Time: " << usedtime << ". Time per test is " << 1.0 * (stop - start) / testnum << "." << std::endl; 


    std::cout << "End of testing the greedy search by using sift" << std::endl;
    delete alg;
}

extern std::vector<Node *> OGS_KDT_Routing(Graph &G,Node *p,Node *q,int l);
extern std::vector<Node *> OGA_routing(Graph &G,std::vector <Node *> C,Node *q,int l);

void test_TOGG_gist(int testnum = 10,int K = 10,int ef = 200,int nb = 8){
    std::fstream out("./logs/TOGG.log",std::ofstream::app);
    time_t start,stop,usedtime = 0;

    std::cout << "Start testing the TOGG search by using gist" << std::endl;
    test_load_data_gist();
    
    std::cout << "Start getting the HNSW Graph" << std::endl;
    alg = build_graph_HNSW(G,gist_base,gist_dim,ef,nb);
    std::cout << alg << std::endl;

    int correct = 0;

    //for (int i = 0; i < N; i++) {
    //    std::priority_queue<std::pair<__type, hnswlib::labeltype>> result = alg->searchKnn((void*)&(G.Nodes[i]->vec[0]), 1);
    //    hnswlib::labeltype label = result.top().second;
    //    if (label == i) correct++;
    //    std::cout << "label is " << label << std::endl;

        //if(debug == 1){
        //    std::cout << G.Nodes[i]->vec[0] << std::endl;
        //}

//    }

    Get_Graph(G,alg);
    std::cout << alg << std::endl;
    std::cout << "End of getting the HNSW Graph" << std::endl;

    //ask for recall
    //recall@K
    for(int i = 0;i < testnum;i ++){

        std::cout << "*" << i  << " *" << G.Nodes[i]->index << std::endl;
        std::priority_queue<std::pair<__type, hnswlib::labeltype>> result = alg->searchKnn((void*)&(G.Nodes[i]->vec[0]), K);
        
        std::set<int> ans;

        std::vector <Node*> testresult;

        start = time(NULL);
        testresult = OGS_KDT_Routing(G,G.Nodes[0].get(),G.Nodes[i].get(),K);
        testresult = OGA_routing(G,testresult,G.Nodes[i].get(),K);
        stop = time(NULL);
        usedtime += stop - start;

        std::cout << "testresult:" << std::endl;
        for(auto tt:testresult){
            ans.insert(tt -> index);
            std::cout << tt -> index << " ";
            std::cout << "dis:" << dis(tt -> vec,G.Nodes[i]->vec) << " ";
        }
        std::cout << std::endl;

        std::cout << "Ansresult:" << std::endl;
        int dd = K;
        while(!result.empty()){
            auto now = result.top(); result.pop();
            std::cout << now.second << " ";
            std::cout << "dis:" << dis(G.Nodes[now.second] -> vec,G.Nodes[i]->vec) << " ";
            if(ans.find(now.second) != ans.end()){
                correct ++;
            }
        }
        std::cout << std::endl;

    //    std::cout << "Now Node:" << std::endl;
    //    std::cout << i <<" " <<  dis(G.Nodes[i] -> vec,G.Nodes[i] -> vec) << std::endl;

    }

    float recall = 1.0 * correct / testnum / K;
    std::cout << "Recall@" << K << " is " << recall << "." << std::endl;
    std::cout << "Handling " << testnum << " queries needs " << usedtime << " seconds " << std::endl;
    out << "" << std::endl;
    out << "Nowtime is " << stop << "." << std::endl;
    out << "Use gist_1M, neighborsize = " << nb << " , ef_construction = " << ef << "." << std::endl;

    out << "testnum = " << testnum << ", K =" << K << "." << std::endl;
   
    out << "Recall@K = " << recall << ". Time: " << usedtime << "s. Time per test is " << 1.0 * (stop - start) / testnum << "s." << std::endl; 

    std::cout << "End of testing the TOGG search by using gist" << std::endl;
    delete alg;
    out.close();

}


void test_TOGG_sift(int testnum = 10,int K = 10,int ef = 200,int nb = 8){
    std::fstream out("./logs/TOGG_FINGER.log",std::ofstream::app);
    time_t start,stop,usedtime = 0;

    std::cout << "Start testing the TOGG search by using gist" << std::endl;
    test_load_data_sift();
    
    std::cout << "Start getting the HNSW Graph" << std::endl;
    alg = build_graph_HNSW(G,sift_base,sift_dim,ef,nb);
    std::cout << alg << std::endl;

    int correct = 0;

    //for (int i = 0; i < N; i++) {
    //    std::priority_queue<std::pair<__type, hnswlib::labeltype>> result = alg->searchKnn((void*)&(G.Nodes[i]->vec[0]), 1);
    //    hnswlib::labeltype label = result.top().second;
    //    if (label == i) correct++;
    //    std::cout << "label is " << label << std::endl;

        //if(debug == 1){
        //    std::cout << G.Nodes[i]->vec[0] << std::endl;
        //}

//    }

    Get_Graph(G,alg);
    std::cout << alg << std::endl;
    std::cout << "End of getting the HNSW Graph" << std::endl;

    //ask for recall
    //recall@K
    for(int i = 0;i < testnum;i ++){

        std::cout << "*" << i << std::endl;
        std::priority_queue<std::pair<__type, hnswlib::labeltype>> result = alg->searchKnn((void*)&(G.Nodes[i]->vec[0]), K);
        
        std::set<int> ans;

        std::vector <Node*> testresult;

        start = time(NULL);
        testresult = OGS_KDT_Routing(G,G.Nodes[0].get(),G.Nodes[i].get(),K);
        testresult = OGA_routing(G,testresult,G.Nodes[i].get(),K);
        stop = time(NULL);
        usedtime += stop - start;

        std::cout << "testresult:" << std::endl;
        for(auto tt:testresult){
            ans.insert(tt -> index);
            std::cout << tt -> index << " ";
            std::cout << "dis:" << dis(tt -> vec,G.Nodes[i]->vec) << " ";
        }
        std::cout << std::endl;

        std::cout << "Ansresult:" << std::endl;
        int dd = K;
        while(!result.empty()){
            auto now = result.top(); result.pop();
            std::cout << now.second << " ";
            std::cout << "dis:" << dis(G.Nodes[now.second] -> vec,G.Nodes[i]->vec) << " ";
            if(ans.find(now.second) != ans.end()){
                correct ++;
            }
        }
        std::cout << std::endl;

    //    std::cout << "Now Node:" << std::endl;
    //    std::cout << i <<" " <<  dis(G.Nodes[i] -> vec,G.Nodes[i] -> vec) << std::endl;

    }

    float recall = 1.0 * correct / testnum / K;
    
    std::cout << "Recall@" << K << " is " << recall << "." << std::endl;
    std::cout << "Handling " << testnum << " queries needs " << usedtime << " seconds " << std::endl;

    out << "" << std::endl;

    out << "Nowtime is " << stop << "." << std::endl;
    out << "Use sift_10000, neighborsize = " << nb << " , ef_construction = " << ef << "." << std::endl;

    out << "testnum = " << testnum << ", K =" << K << "." << std::endl;
    
    out << "Recall@K = " << recall << ". Time: " << usedtime << "s. Time per test is " << 1.0 * (stop - start) / testnum << "s." << std::endl; 

    std::cout << "End of testing the TOGG search by using sift" << std::endl;
    delete alg;
    out.close();

}

extern std::vector<Node *> OGS_KDT_Routing_test1(Graph &G,Node *p,Node *q,int l);
extern std::vector<Node *> OGA_routing_test1(Graph &G,std::vector <Node *> C,Node *q,int l,std::tuple<Eigen::MatrixXf,__type,__type,__type,__type,__type> res);

//Compute parameters
extern std::tuple<Eigen::MatrixXf, __type, __type, __type, __type,__type> computeParameters(
    std::vector <std::shared_ptr <Node> > v, int r);
extern void finger_precalculate_1(Graph &G,std::tuple<Eigen::MatrixXf,__type,__type,__type,__type,__type> res);

//testcase and r
const int test_case = 1000,r = 10;


void test_TOGG_FINGER_gist(int testnum = 10,int K = 10,int ef = 200,int nb = 8){
    std::fstream out("./logs/TOGG_FINGER.log",std::ofstream::app);
    time_t start,stop,usedtime = 0;

    std::cout << "Start testing the TOGG-FINGER search by using gist" << std::endl;
    test_load_data_gist();
    
    std::cout << "Start getting the HNSW Graph" << std::endl;
    alg = build_graph_HNSW(G,gist_base,gist_dim,ef,nb);
    std::cout << alg << std::endl;

    Get_Graph(G,alg);
    std::cout << alg << std::endl;
    std::cout << "End of getting the HNSW Graph" << std::endl;
    
    
    std::vector<std::shared_ptr<Node> > Nodes;
    for(int i = 0 ;i < test_case ;i ++){
        Nodes.push_back(G.Nodes[i]);
    }
    const std::tuple<Eigen::MatrixXf,__type,__type,__type,__type,__type> res = computeParameters(Nodes,r);
    finger_precalculate_1(G,res);

    //ask for recall
    //recall@K
    int correct = 0;
    for(int i = 0;i < testnum;i ++){

        std::cout << "*" << i << std::endl;
        std::priority_queue<std::pair<__type, hnswlib::labeltype>> result = alg->searchKnn((void*)&(G.Nodes[i]->vec[0]), K);
        
        std::set<int> ans;

        std::vector <Node*> testresult;

        start = time(NULL);
        testresult = OGS_KDT_Routing_test1(G,G.Nodes[0].get(),G.Nodes[i].get(),K);
        testresult = OGA_routing_test1(G,testresult,G.Nodes[i].get(),K,res);
        stop = time(NULL);
        usedtime += stop - start;

        std::cout << "testresult:" << std::endl;
        for(auto tt:testresult){
            ans.insert(tt -> index);
            std::cout << tt -> index << " ";
            std::cout << "dis:" << dis(tt -> vec,G.Nodes[i]->vec) << " ";
        }
        std::cout << std::endl;

        std::cout << "Ansresult:" << std::endl;
        int dd = K;
        while(!result.empty()){
            auto now = result.top(); result.pop();
            std::cout << now.second << " ";
            std::cout << "dis:" << dis(G.Nodes[now.second] -> vec,G.Nodes[i]->vec) << " ";
            if(ans.find(now.second) != ans.end()){
                correct ++;
            }
        }
        std::cout << std::endl;

    //    std::cout << "Now Node:" << std::endl;
    //    std::cout << i <<" " <<  dis(G.Nodes[i] -> vec,G.Nodes[i] -> vec) << std::endl;

    }

    float recall = 1.0 * correct / testnum / K;
    std::cout << "Recall@" << K << " is " << recall << "." << std::endl;
    std::cout << "Handling " << testnum << " queries needs " << usedtime << " seconds " << std::endl;

    out << "" << std::endl;
    out << "Nowtime is " << stop << "." << std::endl;
    out << "Use gist_1M, neighborsize = " << nb << " , ef_construction = " << ef << "." << std::endl;

    out << "testnum = " << testnum << ", K =" << K << "." << std::endl;
    
    out << "Recall@K = " << recall << ". Time: " << usedtime << "s. Time per test is " << 1.0 * (stop - start) / testnum << "s." << std::endl; 

    std::cout << "End of testing the TOGG-FINGER search by using gist" << std::endl;
    delete alg;
    out.close();

}

void test_TOGG_FINGER_sift(int testnum = 10,int K = 10,int ef = 200,int nb = 8){
    std::fstream out("./logs/TOGG_FINGER.log",std::ofstream::app);
    time_t start,stop,usedtime = 0;

    std::cout << "Start testing the TOGG-FINGER search by using sift" << std::endl;
    test_load_data_sift();
    
    std::cout << "Start getting the HNSW Graph" << std::endl;
    alg = build_graph_HNSW(G,sift_base,sift_dim,ef,nb);
    std::cout << alg << std::endl;

    Get_Graph(G,alg);
    std::cout << alg << std::endl;
    std::cout << "End of getting the HNSW Graph" << std::endl;
    
    
    std::vector<std::shared_ptr<Node> > Nodes;
    for(int i = 0 ;i < test_case ;i ++){
        Nodes.push_back(G.Nodes[i]);
    }
    const std::tuple<Eigen::MatrixXf,__type,__type,__type,__type,__type> res = computeParameters(Nodes,r);
    finger_precalculate_1(G,res);

    //ask for recall
    //recall@K
    int correct = 0;
    for(int i = 0;i < testnum;i ++){

        std::cout << "*" << i  << " *" << G.Nodes[i]->index << std::endl;
        std::priority_queue<std::pair<__type, hnswlib::labeltype>> result = alg->searchKnn((void*)&(G.Nodes[i]->vec[0]), K);
        
        std::set<int> ans;

        std::vector <Node*> testresult;

        start = time(NULL);
        testresult = OGS_KDT_Routing_test1(G,G.Nodes[0].get(),G.Nodes[i].get(),K);
        testresult = OGA_routing_test1(G,testresult,G.Nodes[i].get(),K,res);
        stop = time(NULL);
        usedtime += stop - start;

        std::cout << "testresult:" << std::endl;
        for(auto tt:testresult){
            ans.insert(tt -> index);
            std::cout << tt -> index << " ";
            std::cout << "dis:" << dis(tt -> vec,G.Nodes[i]->vec) << " ";
        }
        std::cout << std::endl;

        std::cout << "Ansresult:" << std::endl;
        int dd = K;
        while(!result.empty()){
            auto now = result.top(); result.pop();
            std::cout << now.second << " ";
            std::cout << "dis:" << dis(G.Nodes[now.second] -> vec,G.Nodes[i]->vec) << " ";
            if(ans.find(now.second) != ans.end()){
                correct ++;
            }
        }
        std::cout << std::endl;

    //    std::cout << "Now Node:" << std::endl;
    //    std::cout << i <<" " <<  dis(G.Nodes[i] -> vec,G.Nodes[i] -> vec) << std::endl;

    }

    float recall = 1.0 * correct / testnum / K;
    std::cout << "Recall@" << K << " is " << recall << "." << std::endl;
    std::cout << "Handling " << testnum << " queries needs " << usedtime << " seconds " << std::endl;

    out << "" << std::endl;
    out << "Nowtime is " << stop << "." << std::endl;
    out << "Use sift_10000, neighborsize = " << nb << " , ef_construction = " << ef << "." << std::endl;

    out << "testnum = " << testnum << ", K =" << K << "." << std::endl;
    
    out << "Recall@K = " << recall << ". Time: " << usedtime << "s. Time per test is " << 1.0 * (stop - start) / testnum << "s." << std::endl; 

    std::cout << "End of testing the TOGG-FINGER search by using sift" << std::endl;
    delete alg;
    out.close();

}


int main(){
    
//    test_rand();
//    test_build_graph();
//    test_Get_Linklists();
//    test_Get_Graph();
//    test_OGS_KDT_Routing();
//    test_Greedy_Search(10,10,200,16);
//    test_TOGG_gist(10,10,200,16);
//    test_TOGG_sift(100,10,200,16);
    test_TOGG_FINGER_gist(10,10,200,16);
//    test_TOGG_FINGER_sift(10,10,200,64);
    return 0;
}