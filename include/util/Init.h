#ifndef SIMPLE_COMPILER_INIT_H
#define SIMPLE_COMPILER_INIT_H

#include "frontend/Graph.h"
#include "frontend/Nfa.h"

int Graph::NODE_ID = 0;

std::stack<Nfa> Nfa::OP_STACK;


#endif //SIMPLE_COMPILER_INIT_H
