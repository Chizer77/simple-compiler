#ifndef SIMPLE_COMPILER_DFA_H
#define SIMPLE_COMPILER_DFA_H

#include "Graph.h"
#include "Nfa.h"

class Dfa : public Graph{
public:
    /**
     * TODO: DFA最小化
     * @param NFA
     * @return DFA对象
     */
    static Dfa* DfaMinimize(const Dfa& dfa);

    /**
     * TODO: NFA转DFA
     * @param NFA
     * @return DFA对象
     */
    static Dfa* Nfa2Dfa(const Nfa& nfa);

    /**
     * 给定exp生成最小化dfa
     * @param exp 初始正规式
     * @return 最小化dfa对象
     */
    static Dfa* Generation(const std::string& exp);

    ~Dfa() {
        s.clear();
        alpha.clear();
        s0 = 0;
        target.clear();
        edges.clear();
    }
};

#endif //SIMPLE_COMPILER_DFA_H
