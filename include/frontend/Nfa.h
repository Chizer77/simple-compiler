#ifndef SIMPLE_COMPILER_Nfa_H
#define SIMPLE_COMPILER_Nfa_H

#include "string"
#include "stack"
#include "Graph.h"

/**
 * NFA对象
 *
 * 唯一结点序号 NODE_ID
 *
 * 状态结点集合 s
 * 边符号集合 alphabet
 * 初态 s0
 * 终态集合 target
 * 边集合 edges
 */
class Nfa : public Graph{
private:
    static std::stack<Nfa> OP_STACK;

public:

    static const char KLEENE_STATE = '^';
    static const char CONNECTION_STATE = '&';
    static const char UNION_STATE = '|';
    static const char Empty_STATE = '@';

    /**
     * 生成最终的NFA
     * @param exp 初始正规式
     * @return 生成的Nfa
     */
    static Nfa NfaGeneration(const std::string& exp);

    /**
     * 对栈顶一个元素进行闭包^操作，并插入栈顶
     */
    static void Kleene();

    /**
     * 对栈顶两个元素进行连接&操作，并插入栈顶
     */
    static void Connection();

    /**
     * 对栈顶两个元素进行或|操作，并插入栈顶
     */
    static void Union();

    /**
     * 对边c创建NFA，并插入栈顶
     */
    static void NfaInit(char c);

};
#endif //SIMPLE_COMPILER_NfA_H
