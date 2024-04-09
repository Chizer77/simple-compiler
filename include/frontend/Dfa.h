#ifndef SIMPLE_COMPILER_DFA_H
#define SIMPLE_COMPILER_DFA_H

#include "Graph.h"
#include "Nfa.h"

class Dfa : public Graph{
public:
    /**
     * TODO: DFA最小化
     * @param NFA
     * @return DFA
     */
    static Dfa DfaMinimize(Dfa dfa);

    /**
     * TODO: NFA转DFA
     * @param NFA
     * @return DFA
     */
    static Dfa Nfa2Dfa(Nfa nfa);
};

#endif //SIMPLE_COMPILER_DFA_H
