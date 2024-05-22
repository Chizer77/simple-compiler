#ifndef SIMPLE_COMPILER_LL_H
#define SIMPLE_COMPILER_LL_H

#include "CFG.h"
#include "unordered_map"

class LL {
public:

    /**
     * 预测分析表
     */
    std::unordered_map<int, std::unordered_map<int, std::vector<int>>> analysisTable;

    /**
     * TODO:消除左递归
     * @param cfg 文法对象
     * @return 消除左递归后的文法CFG对象
     */
    static CFG* LeftRecurElimination(const CFG& cfg);

    /**
     * TODO:提取左因子
     * @param cfg 文法对象
     * @return 提取左因子后的文法CFG对象
     */
    static CFG* LeftFactorExtraction(const CFG& cfg);

    /**
     * TODO: 求First集合，即求出原文法对象中的firstSet
     * @param cfg 原文法对象
     */
    static void FirstSetSolver(CFG &cfg);

    /**
     * TODO: 求Follow集合，即求出原文法对象中的followSet
     * @param cfg 原文法对象
     */
    static void FollowSetSolver(CFG &cfg);

    /**
     * TODO: 求预测分析表analysisTable，同时判断是否为LL(1)文法
     * @param cfg 求得first和follow集合的文法对象
     * @return 是否为LL(1)文法，0为否
     */
    bool isLLFoundation(const CFG &cfg);
};


#endif //SIMPLE_COMPILER_LL_H
