#ifndef SIMPLE_COMPILER_EDGE_H
#define SIMPLE_COMPILER_EDGE_H

/**
 * 边对象
 *
 * 起点 start
 * 终点 target
 * 符号 alpha,空符号用@表示
 */
class Edge {
public:
    int start;
    int target;
    char alpha;

    Edge(int s, int t, char c) {
        this->start = s;
        this->target = t;
        this->alpha = c;
    }
    ~Edge() = default;

    bool operator <(const Edge & e) const {
        if (start == e.start) return target < e.target;
        return start < e.start;
    }
    bool operator == (const Edge & e) const {
        if (start == e.start && target == e.target && alpha == e.alpha) return true;
        return false;
    }
};
#endif //SIMPLE_COMPILER_EDGE_H
