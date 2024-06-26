#ifndef SIMPLE_COMPILER_INIT_H
#define SIMPLE_COMPILER_INIT_H

#include "../frontend/Graph.h"
#include "../frontend/Nfa.h"
#include "../frontend/Token.h"
#include "../util/FileIO.h"
#include "../frontend/CFG.h"

int Graph::NODE_ID = 0;

std::stack<Nfa*> Nfa::OP_STACK;

FILE *FileIO::input;
FILE *FileIO::output;

std::vector<std::string> Token::list = {
        "IDENT",
        "NUMBER",

        "void",
        "int",
        "float",
        "double",
        "char",

        "break",
        "continue",
        "return",

        "if",
        "else",
        "switch",
        "case",
        "default",

        "for",
        "do",
        "while",

        "&&",
        "||",
        "<=",
        ">=",
        "==",
        "!=",

        "<",
        ">",
        "+",
        "-",
        "*",
        "/",
        "%",
        "!",
        "=",
        ";",
        ",",
        "(",
        ")",
        "[",
        "]",
        "{",
        "}",
        "",
        "SPLIT",
};

int CFG::SYMBOL_ID = (int)Token::list.size() + 10;
int CFG::UNION_ID = (int)Token::list.size() + 1;
int CFG::EMPTY_ID = (int)Token::list.size() + 2;

#endif //SIMPLE_COMPILER_INIT_H
