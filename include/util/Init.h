#ifndef SIMPLE_COMPILER_INIT_H
#define SIMPLE_COMPILER_INIT_H

#include "../frontend/Graph.h"
#include "../frontend/Nfa.h"
#include "FileIO.h"
#include "../frontend/Token.h"

int Graph::NODE_ID = 0;

std::stack<Nfa> Nfa::OP_STACK;

FILE *FileIO::input;
FILE *FileIO::output;

std::string Token::list[45] = {
        "IDENT",
        "NUMBER",

        "VOID",
        "INT",
        "FLOAT",
        "DOUBLE",
        "CHAR",

        "BREAK",
        "CONTINUE",
        "RETURN",

        "IF",
        "ELSE",
        "SWITCH",
        "CHSE",
        "DEFAULT",

        "FOR",
        "DO",
        "WHILE",

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
};


#endif //SIMPLE_COMPILER_INIT_H
