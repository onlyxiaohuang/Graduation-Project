#include "utils.hpp"

struct NeighborTree{
    NeighborTree() = default;
    std::vector <Node *> left,right,common;
    int dim = 0;
};

