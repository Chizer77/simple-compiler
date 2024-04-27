#ifndef SIMPLE_COMPILER_GRAPH_H
#define SIMPLE_COMPILER_GRAPH_H

#include "unordered_set"
#include "Edge.h"

/**
 * 状态节点集 s
 * 边符号集 alpha
 * 开始状态点 s0
 * 终态节点集 target
 * 边集合 edges
 */
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

    static void resetId() {
        NODE_ID = 0;
    }
};

#endif //SIMPLE_COMPILER_GRAPH_H
