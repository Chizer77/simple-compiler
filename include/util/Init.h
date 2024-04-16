#ifndef SIMPLE_COMPILER_INIT_H
#define SIMPLE_COMPILER_INIT_H

#include "../frontend/Graph.h"
#include "../frontend/Nfa.h"
#include "FileIO.h"

int Graph::NODE_ID = 0;

std::stack<Nfa> Nfa::OP_STACK;

FILE *FileIO::input;
FILE *FileIO::output;
char *FileIO::fileStr;

#endif //SIMPLE_COMPILER_INIT_H
