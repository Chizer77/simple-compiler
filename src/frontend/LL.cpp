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
        }
        if (i != productions_list.size()-1) {
            list->push_back(CFG::UNION_ID);
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
            if (it == pre_productions->end()) {
                // 没有间接递归
                entry_productions->push_back(production);
            } else {
                auto pre_list = it->second;
                production->grammar.erase(production->grammar.begin());
                for (auto pre : pre_list) {
                    auto p_vector = new std::vector<int>{production->grammar};
                    p_vector->insert(p_vector->begin(), pre->grammar.begin(), pre->grammar.end());
                    auto p_new = new Productions(production->start, *p_vector);
                    entry_productions->push_back(p_new);
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
    auto m_productions_2 = format(*opg1);

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
            Productions *new_productions = reFormat(no_left_eliminate_productions);
            ans_2.insert(*new_productions);
        } else {
            // 就是将这种格式  P -> Pa|Pb|Pc|Pd|e|f|g|h
            //     * 转成:
            //     * P -> eP'|fP'|gP'|hP'
            //     * p'-> aP'|bP'|cP'|dP'|ε
            //
            // 1.  // 将原来的 P -> e|f|g|h 变成    P -> eP'|fP'|gP'|hP'
            auto *res_productions = new std::vector<Productions*>();
            int new_nonter = CFG::newId();
            opg1->nonter.insert(new_nonter);
            for (Productions *productions : no_left_eliminate_productions) {
                productions->grammar.push_back(new_nonter);
                res_productions->push_back(productions);
            }
            ans_2.insert(*(reFormat(*res_productions)));
            // 2. 将原来的 P -> Pa|Pb|Pc|Pd 变成 P’ -> aP'|bP'|cP'|dP'|ε
            auto *new_res_productions = new std::vector<Productions*>();
            for (Productions *productions : left_eliminate_productions) {
                // 删除左递归第一个
                productions->grammar.erase(productions->grammar.begin());
                // 插入新元素
                productions->grammar.push_back(new_nonter);
                auto *new_p = new Productions(new_nonter, productions->grammar);
                new_res_productions->push_back(new_p);
            }
            std::vector<int> empty_vector({CFG::EMPTY_ID});
            auto *empty = new Productions(new_nonter, empty_vector);
            new_res_productions->push_back(empty);
            ans_2.insert(*(reFormat(*new_res_productions)));
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


        bool factor_something = true;

        while(factor_something)

          for (int nonter : cfg.nonter) {

              std::vector<int> factor;
              std::vector<std::vector<int>> factored_productions;
              std::vector<std::vector<int>> factoring_productions;

               for (const auto& prod1 : cfg.products){
                    if(prod1.start == nonter){
                       factoring_productions.push_back(prod1.grammar);
                    }
               }

             bool factor_something = true;
                    while(factor_something){
                        int mf = 0;
                        for (const auto& prod1 : factoring_productions){
                                for(const auto& prod2 : factoring_productions){
                                    if(prod1.grammar != prod2.grammar){
                                          int minLen = std::min(prod1.size(), prod2.size());

                                          for (int i = 0; i < minLen; i++) {
                                              if (prod1[i] != prod2[i]) {
                                                  break;
                                              }
                                          }

                                          if (i > mf) {
                                            mf = i ;
                                             for (int i = 0; i < minLen; i++) {
                                                  factor.push_back(prod1[i]);
                                             }
                                          }

                                      }
                                    }
                            }

                            for (const auto& prod :factoring_productions){
                                for (int i = 0; i < mf ; i++){
                                    if (prod[i] != factor[i]) {
                                        factored_productions.push_back(prod);
                                        break;
                                    }
                                }
                                if (i == mf){
                                    int newId = cfg.newID();
                                    int Empty = CFG::EMPTY_ID;

                                    std::vector<int> new_production;
                                    new_production.push_back(factor);
                                    new_production.push_back(newId);
                                    opg->products.insert(Productions(nonter, new_production));
                                    //这个因子刚好就是一整个产生式
                                    if (mf == prod.size()){
                                       opg->products.insert(Productions(newId, Empty));
                                    }else{
                                        new_production.clear();
                                       new_production.insert(new_production.end(), prod.begin() + mf, prod.end());
                                    }
                                }
                            }
                            std::swap(factored_productions,factoring_productions);
                            factored_productions.clear();

                            //剩下无法提取左因子的部分
                            if (mf == 0){
                                factor_something = false;
                                 for (const auto& prod :factoring_productions){
                                    opg->products.insert(Productions(nonter, prod));
                                 }
                            }

                        }
                    }

          return opg;
}