#pragma once

#include "utils.hpp"
#include "./libs/hnswlib/hnswlib/hnswlib.h"

hnswlib::HierarchicalNSW<__type>* build_graph_HNSW(Graph &G, int N,int ef_construction,int neighbor_size);//build graph by HNSW

void Get_Graph(Graph &G,hnswlib::HierarchicalNSW<__type>* alg);

void delete_HNSW(hnswlib::HierarchicalNSW<__type>* alg_hnsw);