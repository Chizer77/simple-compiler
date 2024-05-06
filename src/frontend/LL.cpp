#include "frontend/LL.h"
#include "unordered_map"

std::unordered_map<int, std::vector<Productions*>>* format(const CFG& cfg) {
    auto *ans = new std::unordered_map<int, std::vector<Productions*>>;
    for (const Productions& productions : cfg.products) {
        std::vector<Productions*> productions_list;
        std::vector<int> list;
        for (int grammer : productions.grammar) {
            if (grammer == CFG::UNION_ID) {
                auto *productions_new = new Productions(productions.start, list);
                productions_list.push_back(productions_new);
                list.clear();
            } else {
                list.push_back(grammer);
            }
        }
        auto *productions_new = new Productions(productions.start, list);
        productions_list.push_back(productions_new);
        ans->insert({productions_new->start, productions_list});
    }
    return ans;
}


Productions* reFormat(std::vector<Productions*>& productions_list) {
    auto *list = new std::vector<int>();
    for (int i = 0; i < productions_list.size(); i++) {
        for (int garmmer : productions_list[i]->grammar) {
            list->push_back(garmmer);
            if (i != productions_list.size()-1) {
                list->push_back(CFG::UNION_ID);
            }
        }
    }
    auto *ans = new Productions(productions_list[0]->start, *list);
    return ans;
}

// TODO:free 内存
CFG* LL::LeftRecurElimination(const CFG& cfg) {
    CFG *opg1 = new CFG();
    auto m_productions = format(cfg);

    // 1. 消除间接左递归
    //     * 如同 S -> Aa|b   A -> Ac|Sd
    //     * 会产生  S ==> Aa ==> Sda
    //     * 消除间接左递归变成
    //     * S -> Aa|b
    //     * A -> Ac|Aad|bd

    std::unordered_set<Productions, Productions::ProductionsHasher> ans_1;
    // 记录之前遍历过的 productions
    auto *pre_productions = new std::unordered_map<int, std::vector<Productions*>>();
    for (auto productions_pair = m_productions->begin(); productions_pair != m_productions->end(); ++productions_pair) {
        auto entry_productions = new std::vector<Productions*>();
        for (auto *production : productions_pair->second) {
            int rightFirst = production->grammar[0];
            auto it = pre_productions->find(rightFirst);
            if (it == nullptr) {
                // 没有间接递归
                entry_productions->push_back(production);
            } else {
                auto pre_list = it->second;
                for (auto pre : pre_list) {
                    production->grammar.erase(production->grammar.begin());
                    production->grammar.insert(production->grammar.begin(), pre->grammar.begin(), pre->grammar.end());
                }
            }
        }
        pre_productions->insert({productions_pair->first, *entry_productions});
        ans_1.insert(*(reFormat(*entry_productions)));
    }

    opg1->nonter = cfg.nonter;
    opg1->start = cfg.start;
    opg1->ter = cfg.ter;
    opg1->products = ans_1;

    // 2. 消除直接左递归
    CFG *opg2 = new CFG();
    auto m_productions_2 = format(*opg2);

    std::unordered_set<Productions, Productions::ProductionsHasher> ans_2;
    for (auto productions_pair = m_productions_2->begin(); productions_pair != m_productions_2->end(); ++productions_pair) {
        std::vector<Productions*> no_left_eliminate_productions;
        std::vector<Productions*> left_eliminate_productions;
        for  (Productions *productions : productions_pair->second) {
            // 直接递归
            if (productions->grammar[0] == productions->start) {
                left_eliminate_productions.push_back(productions);
            } else {
                no_left_eliminate_productions.push_back(productions);
            }
        }
        // 都不包含左递归
        if (left_eliminate_productions.empty()) {
            Productions *new_productions = reFormat(left_eliminate_productions);
            ans_2.insert(*new_productions);
        } else {
           auto *res_productions = new std::vector<Productions*>();
            // 就是将这种格式  P -> Pa|Pb|Pc|Pd|e|f|g|h
            //     * 转成:
            //     * P -> eP'|fP'|gP'|hP'
            //     * p'-> aP'|bP'|cP'|dP'|ε
            //
            // 1.  // 将原来的 P -> e|f|g|h 变成    P -> eP'|fP'|gP'|hP'
            int new_nonter = CFG::newId();
            opg1->nonter.insert(new_nonter);
            for (Productions *productions : no_left_eliminate_productions) {
                productions->grammar.push_back(new_nonter);
                res_productions->push_back(productions);

            }
            // 2. 将原来的 P -> Pa|Pb|Pc|Pd 变成 P’ -> aP'|bP'|cP'|dP'|ε
            for (Productions *productions : left_eliminate_productions) {
                // 删除左递归第一个
                productions->grammar.erase(productions->grammar.begin());
                // 插入新元素
                productions->grammar.push_back(new_nonter);
                auto *new_p = new Productions(new_nonter, productions->grammar);
                res_productions->push_back(productions);
            }
            std::vector<int> empty_vector(CFG::EMPTY_ID);
            auto *empty = new Productions(new_nonter, empty_vector);
            res_productions->push_back(empty);

            ans_2.insert(*(reFormat(*res_productions)));
        }
    }

    opg2->nonter = opg1->nonter;
    opg2->start = opg1->start;
    opg2->ter = opg1->ter;
    opg2->products = ans_2;

    return opg2;
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