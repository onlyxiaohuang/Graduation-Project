#pragma once

#include "utils.hpp"
#include "./libs/hnswlib/hnswlib/hnswlib.h"

hnswlib::HierarchicalNSW<__type>* build_graph_HNSW(Graph &G);
void delete_HNSW(hnswlib::HierarchicalNSW<__type>* alg_hnsw);