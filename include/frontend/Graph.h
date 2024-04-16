#ifndef SIMPLE_COMPILER_GRAPH_H
#define SIMPLE_COMPILER_GRAPH_H

#include "unordered_set"
#include "Edge.h"

class Graph {
private:
    static int NODE_ID;
public:
    std::unordered_set<int> s;
    std::unordered_set<char> alpha;
    int s0{};
    std::unordered_set<int> target;
    std::unordered_set<Edge, Edge::EdgeHasher> edges;

    Graph() = default;
    ~Graph() {
        s.clear();
        alpha.clear();
        s0 = 0;
        target.clear();
        edges.clear();
    }

    static int newId() {
        return NODE_ID++;
    }
};

#endif //SIMPLE_COMPILER_GRAPH_H
