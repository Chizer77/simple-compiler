#ifndef SIMPLE_COMPILER_INIT_H
#define SIMPLE_COMPILER_INIT_H

#include "frontend/nfa/Nfa.h"

int Nfa::NODE_ID = 0;

std::stack<Nfa> Nfa::OP_STACK;


#endif //SIMPLE_COMPILER_INIT_H
