#include "frontend/LL.h"



CFG* LL::LeftRecurElimination(const CFG& cfg) {
    CFG *opg = new CFG();
    return opg;
}



CFG* LL::LeftFactorExtraction(const CFG& cfg) {
     CFG* opg = new CFG();

        opg->start = cfg.start;
        opg->nonter = cfg.nonter;
        opg->ter = cfg.ter;

        for (const auto& prod : cfg.products) {
            int union_id = cfg::UNION_ID;

                std::vector<std::vector<int>> factorizing_productions;
                std::vector<std::vector<int>> factorized_productions;
                std::vector<std::vector<int>> factors;

                bool factorized_something = false;

                    if (find(opg.nonter.begin(),opg.nonter.end(),prod.start)!= opg.nonter.end()){//判断是否为非终结符

                        for(auto& element : prod.grammar){
                            if (element == union_id || element == prod.grammar.end()-1){
                                if (factorized_productions.empty()) {
                                  factorized_productions.push_back(factorizing_productions);
                                 } else {
                                 bool factorized = false;
                                    size_t min_size = std::min(factorizing_productions.size(), factorized_productions .size());
                                    size_t matching_elements = 0;

                                        for (size_t i = 0; i < min_size; ++i) {
                                            if (factorizing_productions[i] == factorized_productions[i]) {
                                                matching_elements++;
                                            } else {
                                                break;
                                            }
                                        }

                                        if (matching_elements > 0) {
                                            factorized = true;

                                            newId = cfg.newId();
                                            factor.insert(factor.end(),factorizing_productions.begin(),factorizing_productions.begin() + matching_elements - 1);
                                            factor.insert(factor.end(), newId);

                                            factorized_productions.insert(factorized_productions.end(), factorizing_productions.begin() + matching_elements, factorizing_productions.end());
                                            factorized_productions.insert(factorized_productions.end(), union_id);
                                            factorized_something = true;
                                        }

                                        if(!factorized){
                                            factor.insert(factor.end(),union_id)
                                            factor.insert(factor.end(),factorizing_productions);
                                        }

                                 }
                                factorizing_productions.clear();//清空为什么没有提示？
                            }else{
                                factorizing_productions.push_back(element);
                            }
                            }
                          if (factorized_something) {
                                   opg->nonter.insert(opg->nonter.end(),newId)
                                   opg->products.insert(Productions(newId, factorized_productions ));
                                   opg->products.insert(Productions(prod.start, factor));
                            }
                        }
                        }
//存在bug：只能识别与第一个产生式有共同因子的产生式；无法提取多个不同的因子；解决办法：可以设置一个while，在对比玩第一个产生式后删除掉它，直到只剩下一个产生式，但是这样数组不好弄
        return opg;

}