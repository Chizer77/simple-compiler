#ifndef SIMPLE_COMPILER_INIT_H
#define SIMPLE_COMPILER_INIT_H

#include "../frontend/Graph.h"
#include "../frontend/Nfa.h"
#include "FileReader.h"

int Graph::NODE_ID = 0;

std::stack<Nfa> Nfa::OP_STACK;

FILE *FileReader::input;
FILE *FileReader::output;
char *FileReader::fileStr;

#endif //SIMPLE_COMPILER_INIT_H
