#ifndef SIMPLE_COMPILER_Nfa_H
#define SIMPLE_COMPILER_Nfa_H

#include "set"
#include "string"
#include "Edge.h"
#include "stack"

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
class Nfa {

private:
    static int NODE_ID;
    static std::stack<Nfa> OP_STACK;
    std::set<int> s;
    std::set<char> alpha;
    int s0{};
    std::set<int> target;
    std::set<Edge> edges;

    /**
     * TODO:对栈顶一个元素进行闭包^操作，并插入栈顶
     */
    static void Kleene();

    /**
     * TODO:对栈顶两个元素进行连接*操作，并插入栈顶
     */
    static void Concatenation();

    /**
     * 对栈顶两个元素进行或|操作，并插入栈顶
     */
    static void Union();

    /**
     * 对边c创建NFA，并插入栈顶
     */
    static void NfaInit(char c);

public:
    Nfa();

    /**
     * 生成最终的NFA
     * @param exp 初始正规式
     * @return 生成的Nfa
     */
    static Nfa NfaGeneration(const std::string& exp);

    ~Nfa();
};
#endif //SIMPLE_COMPILER_NfA_H