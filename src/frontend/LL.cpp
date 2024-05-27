#include "frontend/LL.h"
#include "unordered_map"

std::unordered_map<int, std::vector<Productions*>>* format(const CFG& cfg) {
    auto *ans = new std::unordered_map<int, std::vector<Productions*>>;
    for (const Productions& productions : cfg.products) {
        std::vector<int> list;
        std::vector<Productions*> productions_list;
        auto ans_item = ans->find(productions.start);
        if(ans_item != ans->end()) {
            productions_list = ans_item->second;
            ans->erase(productions.start);
        }
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
        ans->insert({productions.start, productions_list});
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
    for (auto & m_production : *m_productions) {
        auto entry_productions = new std::vector<Productions*>();
        for (auto *production : m_production.second) {
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
        pre_productions->insert({m_production.first, *entry_productions});
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
    for (auto & productions_pair : *m_productions_2) {  //遍历每个非终结符
        std::vector<Productions*> no_left_eliminate_productions;
        std::vector<Productions*> left_eliminate_productions;
        for  (Productions *productions : productions_pair.second) { //遍历同一非终结符下的产生式
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


std::vector<std::vector<int>> subExtraction(CFG *cfg, int start, int newId, std::vector<int>& factor, std::vector<std::vector<int>>& factoring_productions) {
    // <start> -> <maxFactor><newId>
    std::vector<int> new_production(factor);    // 新产生式右边
    new_production.push_back(newId);
    cfg->products.insert(Productions(start, new_production));
    cfg->nonter.insert(newId);

    // <newId> -> XXX | XX
    std::vector<std::vector<int>> newFactoring_prod;
    for (auto &prod: factoring_productions) {
        prod.erase(prod.begin(), prod.begin() + (int)factor.size());
        //这个因子刚好就是一整个产生式
        if (prod.empty()) {
            std::vector<int> emptyProd{CFG::EMPTY_ID};
            cfg->products.insert(Productions(newId, emptyProd));
        }else {
            newFactoring_prod.push_back(prod);
        }
    }
    return newFactoring_prod;
}

void partPrefixExtraction(CFG *cfg, int start, const std::vector<std::vector<int>>& factoring_productions) {
    std::vector<std::vector<int>> fact_prod(factoring_productions);
    while (true) {
        //提取有公共前缀的部分产生式，取尽量包含更多产生式的公共前缀
        std::vector<std::vector<int>> hasFactor_prod;
        std::vector<int> hasFactor;    //该公共前缀
        std::vector<int> hasFactorPos; //该公共前缀在待处理产生式中的下标位置
        for(const auto& prod1: fact_prod) {
            for(const auto& prod2: fact_prod) {
                if(prod1 == prod2) continue;
                int len = prod1.size() <= prod2.size() ? (int)prod2.size() : (int)prod1.size();
                int idx = 0;
                while(idx < len) {
                    if(prod1[idx] != prod2[idx]) break;
                    idx++;
                }
                if(idx == 0) continue;
                //部分公共前缀
                std::vector<int> partFactor(prod1.begin(), prod1.begin() + idx);
                //包含该前缀的产生式
                std::vector<std::vector<int>> partProd;
                std::vector<int> factorPos; //这些产生式在待处理产生式中的下标位置
                for(int i = 0; i < fact_prod.size(); i++) {
                    std::vector<int> prod3 = fact_prod[i];
                    if(prod3.size() < partFactor.size()) continue;
                    int isOK = true;
                    int pos = 0;
                    while (pos < partFactor.size()) {
                        if(prod3[pos] != partFactor[pos]) {
                            isOK = false;
                            break;
                        }
                        pos++;
                    }
                    if(isOK) {
                        partProd.push_back(prod3);
                        factorPos.push_back(i);
                    }
                }
                if(partProd.size() > hasFactor_prod.size()) {
                    hasFactor_prod.swap(partProd);
                    hasFactor.swap(partFactor);
                    hasFactorPos.swap(factorPos);
                }
            }
        }
        //找不到则直接退出
        if(hasFactor.empty()) {
            for (auto prod: fact_prod) {
                cfg->products.insert(Productions(start, prod));
            }
            break;
        }
        int newId = CFG::newId();
        // <start> -> <maxFactor><newId>
        // <newId> -> XXX | XX
        std::vector<std::vector<int>> newFactoring_prod = subExtraction(cfg, start, newId, hasFactor, hasFactor_prod);
        //寻找提取完左因子后的产生式的新部分前缀
        partPrefixExtraction(cfg, newId, newFactoring_prod);
        //删除处理完的产生式
        int ct = 0;
        for(int it: hasFactorPos) {
            fact_prod.erase(fact_prod.begin() + it - ct);
            ct++;
        }
    }
}

CFG *LL::LeftFactorExtraction(const CFG &cfg) {
    CFG *opg = new CFG();
    opg->start = cfg.start;
    opg->nonter = cfg.nonter;
    opg->ter = cfg.ter;

    for (int nonter: cfg.nonter) {
        int start = nonter; //当前处理的产生式左边
        std::vector<std::vector<int>> factoring_productions;    // 正在提取左因子的产生式
        for (const auto &prod: cfg.products) {
            if (prod.start == start) {
                factoring_productions.push_back(prod.grammar);
            }
        }
        if(factoring_productions.empty()) continue;
        // 寻找公共前缀
        std::vector<int> maxFactor = *factoring_productions.begin();
        for(auto prod: factoring_productions) {
            int idx = 0;
            while(idx < maxFactor.size() && idx < prod.size()) {
                if(prod[idx] != maxFactor[idx]) break;
                idx++;
            }
            maxFactor.erase(maxFactor.begin() + idx, maxFactor.end());
        }
        //提取公共前缀
        if(!maxFactor.empty()) {
            int newId = CFG::newId();
            // <start> -> <maxFactor><newId>
            // <newId> -> XXX | XX
            std::vector<std::vector<int>> newFactoring_prod = subExtraction(opg, start, newId, maxFactor, factoring_productions);
            //更新待处理的产生式
            factoring_productions.clear();
            factoring_productions.swap(newFactoring_prod);
            //更新需处理的产生式的左边
            start = newId;
        }
        // 寻找部分公共前缀，并取尽量包含更多产生式的公共前缀，再寻找符合该公共前缀的其他产生式
        partPrefixExtraction(opg, start, factoring_productions);
    }
    return opg;
}

void dfsFirst(std::vector<Productions*> &items, std::unordered_map<int, std::vector<int>> *ans, std::unordered_map<int, std::vector<Productions*>> *map, const CFG &cfg) {
    auto ans_vector = new std::vector<int>;
    for (auto item : items) {
        int rightFirst = item->grammar[0];
        // 非终结符
        if (cfg.ter.find(rightFirst) == cfg.ter.end()) {
            if (rightFirst == CFG::EMPTY_ID) {
                ans_vector->push_back(rightFirst);
                break;
            }
            // 还没完成
            if (ans->find(rightFirst) == ans->end()) {
                dfsFirst(map->find(rightFirst)->second, ans, map, cfg);
            }
            auto v = ans->find(rightFirst);
            for (auto v_item : v->second) {
                ans_vector->push_back(v_item);
            }
            // 终结符
        } else {
            ans_vector->push_back(rightFirst);
        }
    }
    ans->insert({items[0]->start, *ans_vector});
}

// TODO:free
void LL::FirstSetSolver(CFG &cfg) {
    auto m = format(cfg);
    auto ans = new std::unordered_map<int, std::vector<int>>;
    for (auto item : *m) {
        dfsFirst(item.second, ans, m, cfg);
    }
    auto set = new std::unordered_set<SubSet, SubSet::SubSetHasher>;
    for (auto item : *ans) {
        auto sub_set = new SubSet;
        sub_set->symbol = item.first;
        for (auto item_int : item.second) {
            sub_set->st.insert(item_int);
        }
        set->insert(*sub_set);
    }
    cfg.firstSet = *set;
}

int is_change(std::unordered_map<int, std::unordered_set<int>>& fol_m,int symbol,int ist){
    if(fol_m.find(symbol) == fol_m.end()) {
        // 如果键不存在，创建一个新的unordered_set
        std::unordered_set<int> newSet;
        newSet.insert(ist);
        fol_m[symbol] = newSet;
        return true;
    } else {
        // 如果键存在，直接插入值
        if(fol_m[symbol].find(ist) == fol_m[symbol].end()){//判断值是否已经有过
            fol_m[symbol].insert(ist);
            return true;
        }else{return false;}
    }

    return false;
}

std::unordered_set<int> getSetBySymbol(std::unordered_set<SubSet, SubSet::SubSetHasher>& cfgSet, int symbol) {
    std::unordered_set<int> Set;
    for(const auto& subset : cfgSet) {
        if(subset.symbol == symbol) {
            Set.insert(subset.st.begin(), subset.st.end());
        }
    }
    return Set;
}


void LL::FollowSetSolver(CFG &cfg) {
    std::unordered_map<int, std::unordered_set<int>> fol_m;
    //规则一：加入#号

    bool change = is_change(fol_m,cfg.start,CFG::Terminal_ID);

    while(change){
            change = false;
            for (const auto &prod: cfg.products) {
                    for(int i = 0; i < prod.grammar.size();i++){
                        int j = i;
                        int s = prod.grammar[j];
                        int next_s = prod.grammar[j+1];
                        if(cfg.nonter.find(s) != cfg.nonter.end()&& i != prod.grammar.size()){//规则二：找到一个非终结符
                                if(cfg.ter.find(next_s) != cfg.ter.end())//非终结符后的终结符
                                {
                                    change = is_change(fol_m,s,next_s);
                                }
                            for(;j<prod.grammar.size();j++){
                                if(cfg.nonter.find(next_s) != cfg.nonter.end())//非终结符后的非终结符
                                {
                                    std::unordered_set<int> fir_set;
                                    fir_set = getSetBySymbol(cfg.firstSet,next_s);
                                    for(auto const id: fir_set){
                                        if(id != CFG::EMPTY_ID){
                                            change = is_change(fol_m,s,id);
                                        }
                                    }

                                    if(fir_set.find(CFG::EMPTY_ID) == fir_set.end()){
                                        break;
                                    }//first集除了空全给
                                }
                            }
                        }

                        if(change){
                            for(auto & it : fol_m){
                                SubSet Follow{it.first,it.second};
                                cfg.followSet.insert(Follow);
                            }
                        }
                    }

                int las = prod.grammar.back();

                if(cfg.nonter.find(las) != cfg.nonter.end()){//规则三：非终结符的follow给非终结符

                    std::unordered_set<int> fol_set;
                    std::unordered_set<int> fir_set;

                    fol_set = getSetBySymbol(cfg.followSet,prod.start);

                    for(auto const id: fol_set){
                        change = is_change(fol_m,las,id);
                    }

                    for(int sj = 0; sj < prod.grammar.size();sj++){//非终结符的first集可能存在空
                        fir_set = getSetBySymbol(cfg.firstSet,las);
                        int pr_las = prod.grammar[sj - 1];

                        if(fir_set.find(CFG::EMPTY_ID) != fir_set.end()){
                            if(cfg.nonter.find(pr_las) != cfg.nonter.end()){
                                for(auto const id : fol_set){
                                    change = is_change(fol_m,pr_las,id);
                                }
                            }
                        }else{break;}

                    }

                    //follow集全给
                    if(change){
                        for(auto & it : fol_m){
                            SubSet Follow{it.first,it.second};
                            cfg.followSet.insert(Follow);
                        }
                    }
                }

            }

    }

}

bool LL::isLLFoundation(const CFG &cfg) {
    return false;
}
