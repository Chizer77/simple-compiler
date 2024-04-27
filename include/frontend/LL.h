#ifndef SIMPLE_COMPILER_LL_H
#define SIMPLE_COMPILER_LL_H

#include "CFG.h"

class LL {
public:

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

};


#endif //SIMPLE_COMPILER_LL_H
