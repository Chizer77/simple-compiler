#include "frontend/Dfa.h"
#include <queue>

Dfa Dfa::DfaMinimize(Dfa& dfa) {
    return {};
}

Dfa Dfa::Nfa2Dfa(Nfa& nfa) {
    DFA dfa;

    //子集构造
    std::unordered_set<std::unordered_set<int>> dfaStates;
    std::queue<std::unordered_set<int>> stateQueue;

    //计算NFA初始状态的空闭包
    std::unordered_set<int> initialStateClosure = epsilonClosure(nfa.s0);

    dfa.s.insert(Graph::NODE_ID++);
    dfa.s0 = Graph::NODE_ID - 1;

    dfaStates.insert(initialStateClosure);
    stateQueue.push(initialStateClosure);

   // 进行状态转移和构建DFA状态集合
   while (!stateQueue.empty()) {
       std::unordered_set<int> currentState = stateQueue.front();
       stateQueue.pop();

       for (char symbol : dfa.alpha) {
           std::unordered_set<int> nextState = move(nfa, currentState, symbol);
           nextState = epsilonClosure(nextState);

           if (!nextState.empty() && dfaStates.find(nextState) == dfaStates.end()) {
               dfaStates.insert(nextState);
               dfa.s.insert(Graph::NODE_ID++);
               stateQueue.push(nextState);
           }

           // 在DFA中添加从currentState到nextState的symbol转移
           if (!nextState.empty()) {
               Edge edge;
               edge.from = currentState;
               edge.to = nextState;
               edge.symbol = symbol;
               dfa.edges.insert(edge);
           }
       }
   }

   // 设置DFA的目标状态
   for (const auto& state : dfaStates) {
       for (int targetState : nfa.target) {
           if (state.find(targetState) != state.end()) {
               dfa.target.insert(Graph::NODE_ID);
               break;
           }
       }
   }

   return dfa;
}

