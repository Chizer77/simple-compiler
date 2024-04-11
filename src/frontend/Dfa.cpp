#include <algorithm>
#include "frontend/Dfa.h"

Dfa Dfa::DfaMinimize(Dfa& dfa) {
    //定义各结点状态
    std::unordered_map<int, int> state;
    int ps = 0;
    for(int ts: dfa.s) state[ts] = ps;
    ps++;
    for(int te: dfa.target) state[te] = ps;
    //映射各边
    std::unordered_map<int, std::unordered_map<char, int>> edgesMap;
    for(Edge e: dfa.edges) edgesMap[e.start][e.alpha] = e.target;
    //划分
    std::vector<int> nodes(dfa.s.begin(), dfa.s.end());
    int isCycle = 1;
    while (isCycle) {
        isCycle = 0;
        ps++;
        for(int i = 0; i < nodes.size(); i++) {
            for(int j = i + 1; j < nodes.size(); j++) {
                //只检查当前在同一集合的两结点
                if(state[nodes[i]] != state[nodes[j]]) continue;
                //检查两结点的下个状态是否完全相同，相同则isEqual=1，不同则isEqual=0
                int isEqual = 1;
                //边数不同直接isEqual=0
                if(edgesMap[nodes[i]].size() != edgesMap[nodes[j]].size()) isEqual = 0;
                else {
                    for(std::pair<char, int> e: edgesMap[nodes[i]]) {
                        char ap = e.first;
                        int t = e.second;
                        if(edgesMap[nodes[j]].count(ap) == 0 || state[t] != state[edgesMap[nodes[j]][ap]]) {
                            isEqual = 0;
                            break;
                        }
                    }
                }
                if(!isEqual) {
                    state[nodes[j]] = ps;
                    isCycle = 1;
                }
            }
        }
    }
    // 重新标号,生成中间产物preDfa
    Dfa *preDfa = new Dfa();
    for(int os: dfa.s) preDfa->s.insert(state[os]);
    for(char ap: dfa.alpha) preDfa->alpha.insert(ap);
    preDfa->s0 = state[dfa.s0];
    for(int tar: dfa.target) preDfa->target.insert(state[tar]);
    for(Edge e: dfa.edges) {
        preDfa->edges.insert(*(new Edge(state[e.start], state[e.target], e.alpha)));
    }
    // 重新映射preDfa各边
    edgesMap.clear();
    for(Edge e: preDfa->edges) edgesMap[e.start][e.alpha] = e.target;
    // 检查死状态
    //1. 删除自身不是终态，但所有边都指向自身的状态
    //2. 从初态开始dfs，删除不可到达的状态和其所包含边
    //3. 直接将state设置为-1，在之后构造最小集时选择不插入
    std::unordered_map<int, int> visit;
    std::stack<int> checkStack;
    checkStack.push(preDfa->s0);
    while(!checkStack.empty()) {
        int nowNode = checkStack.top();
        checkStack.pop();
        if(visit.count(nowNode) == 1) continue;
        visit[nowNode] = 1;
        for(std::pair<char, int> e: edgesMap[nowNode]) {
            int nextNode = e.second;
            if(preDfa->target.find(nextNode) == preDfa->target.end()) {
                int isDeadState = 1;
                for(std::pair<char, int> ne: edgesMap[nextNode]) {
                    if(ne.second != nextNode) isDeadState = 0;
                }
                // 删除死状态，且非初态(正常都是非初态)
                if(isDeadState && nextNode != preDfa->s0) visit[nextNode] = -1;
            }
            checkStack.push(e.second);
        }
    }
    // 删除不可到达的状态和死状态==>visit == 0 || == -1
    Dfa *minDfa = new Dfa();
    for(int os: preDfa->s) {
        if(visit[os] == 1) {
            minDfa->s.insert(os);
        }
    }
    minDfa->s0 = preDfa->s0;
    for(int tar: preDfa->target) {
        if(visit[tar] == 1) {
            minDfa->target.insert(tar);
        }
    }
    for(Edge e: preDfa->edges) {
        if(visit[e.start] == 1 && visit[e.target] == 1) {
            minDfa->edges.insert(*(new Edge(e.start, e.target, e.alpha)));
            minDfa->alpha.insert(e.alpha);
        }
    }
    return *minDfa;
}

Dfa Dfa::Nfa2Dfa(Nfa& nfa) {
    return {};
}
