#ifndef SIMPLE_COMPILER_GRAPH_H
#define SIMPLE_COMPILER_GRAPH_H

#include "set"
#include "Edge.h"

class Graph {
public:
    static int NODE_ID;
    std::set<int> s;
    std::set<char> alpha;
    int s0{};
    std::set<int> target;
    std::set<Edge> edges;

    Graph() = default;
    ~Graph() {
        s.clear();
        alpha.clear();
        s0 = 0;
        target.clear();
        edges.clear();
    }
};

#endif //SIMPLE_COMPILER_GRAPH_H
