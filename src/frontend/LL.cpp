#include "queue"
#include "frontend/LL.h"
#include "unordered_map"


std::unordered_map<int, std::unordered_map<int, std::vector<int>>> LL::analysisTable;


void LL::Split(CFG &cfg) {
    std::unordered_set<Productions, Productions::ProductionsHasher> splitProd;
    std::vector<int> list;
    for (const Productions &prod: cfg.products) {
        for (int g: prod.grammar) {
            if (g == CFG::UNION_ID) {
                splitProd.insert(Productions(prod.start, list));
                list.clear();
            } else {
                list.push_back(g);
            }
        }
        if (!list.empty()) {
            splitProd.insert(Productions(prod.start, list));
            list.clear();
        }
    }
    cfg.products = splitProd;
}

std::unordered_map<int, std::vector<Productions *>> *format(const CFG &cfg) {
    auto *ans = new std::unordered_map<int, std::vector<Productions *>>;
    for (Productions prod: cfg.products) {
        std::vector<Productions *> list;
        auto tmp = ans->find(prod.start);
        if (tmp != ans->end()) {
            list = tmp->second;
            ans->erase(tmp);
        }
        list.push_back(new Productions(prod.start, prod.grammar));
        ans->insert({prod.start, list});
    }
    return ans;
}

Productions *reFormat(std::vector<Productions *> &productions_list) {
    auto *list = new std::vector<int>();
    for (int i = 0; i < productions_list.size(); i++) {
        for (int garmmer: productions_list[i]->grammar) {
            list->push_back(garmmer);
        }
        if (i != productions_list.size() - 1) {
            list->push_back(CFG::UNION_ID);
        }
    }
    auto *ans = new Productions(productions_list[0]->start, *list);
    return ans;
}

void exposure(int start, std::vector<int> g, std::unordered_set<Productions, Productions::ProductionsHasher> &ans,
              std::unordered_map<int, std::vector<Productions *>> &m_productions,
              std::unordered_set<Productions, Productions::ProductionsHasher> &visited, const CFG &cfg, int& f) {
    // f 表示是否成功暴露了左递归
    std::vector<int> gt(g);
    if (gt.empty()) { //本身是空的
        gt.emplace_back(CFG::EMPTY_ID);
        ans.insert(Productions(start, gt));
        return;
    }
    if (cfg.nonter.find(*gt.begin()) == cfg.nonter.end()) { //首字为终结符或空符号
        ans.insert(Productions(start, gt));
        return;
    }
    auto pv = m_productions.find(*gt.begin())->second;
    for (auto prod: pv) {
        gt = g;
        if (visited.find(*prod) != visited.end()) { //下一条边已跑过
//            ans.insert(Productions(start, gt));
        } else if (cfg.nonter.find(prod->grammar[0]) == cfg.nonter.end() &&
                   prod->grammar[0] != CFG::EMPTY_ID) {   //下条边首字为终结符
            visited.insert(*prod);
//            gt.erase(gt.begin());
//            gt.insert(gt.begin(), prod->grammar.begin(), prod->grammar.end());
//            ans.insert(Productions(start, gt));
        } else if (prod->grammar[0] == start) {   //找到左递归，那么插入暴露后的产生式
            visited.insert(*prod);
            for(auto pp: pv) { // 需要完整插入
                gt.erase(gt.begin());
                gt.insert(gt.begin(), pp->grammar.begin(), pp->grammar.end());
                gt = g;
                ans.insert(Productions(start, gt));
            }
            f = 1;
        } else if (prod->grammar[0] == CFG::EMPTY_ID) {    //首字为空
            gt.erase(gt.begin());
            exposure(start, gt, ans, m_productions, visited, cfg, f);
        } else { //其他非终结符
            visited.insert(*prod);
            gt.erase(gt.begin());
            gt.insert(gt.begin(), prod->grammar.begin(), prod->grammar.end());
            exposure(start, gt, ans, m_productions, visited, cfg, f);
        }
    }
}


// TODO:free 内存
CFG *LL::LeftRecurElimination(const CFG &cfg) {
    CFG *opg1 = new CFG();
    std::unordered_set<Productions, Productions::ProductionsHasher> regProd;
    auto m_productions = format(cfg);
    std::unordered_set<Productions, Productions::ProductionsHasher> visited;
    // 1. 暴露间接左递归
    //     * 如同 S -> Aa|b   A -> Ac|Sd
    //     * 会产生  S ==> Aa ==> Sda
    //     * 消除间接左递归变成
    //     * S -> Aa|b
    //     * A -> Ac|Aad|bd
    // 相互间接左递归时，只需要解析一方
    for (int t: cfg.nonter) {
        visited.clear();
        auto pv = m_productions->find(t)->second;
        for (auto p: pv) {
            if (p->grammar[0] != CFG::EMPTY_ID) visited.insert(*p);
            int f = 0;
            exposure(t, (p->grammar), regProd, *m_productions, visited, cfg, f);
            if(!f) regProd.insert(Productions(t, (p->grammar)));
        }
    }

    opg1->nonter = cfg.nonter;
    opg1->start = cfg.start;
    opg1->ter = cfg.ter;
    opg1->products = regProd;

    // 2. 消除直接左递归
    CFG *opg2 = new CFG();
    auto m_productions_2 = format(*opg1);

    std::unordered_set<Productions, Productions::ProductionsHasher> ans_2;
    for (auto &productions_pair: *m_productions_2) {  //遍历每个非终结符
        std::vector<Productions *> no_left_eliminate_productions;
        std::vector<Productions *> left_eliminate_productions;
        for (Productions *productions: productions_pair.second) { //遍历同一非终结符下的产生式
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
            auto *res_productions = new std::vector<Productions *>();
            int new_nonter = CFG::newId();
            opg1->nonter.insert(new_nonter);
            for (Productions *productions: no_left_eliminate_productions) {
                if (productions->grammar[0] == CFG::EMPTY_ID) productions->grammar.erase(productions->grammar.begin());
                //加上新非终结符 P'
                productions->grammar.push_back(new_nonter);
                res_productions->push_back(productions);
            }
            if(!res_productions->empty()) ans_2.insert(*(reFormat(*res_productions)));
            // 2. 将原来的 P -> Pa|Pb|Pc|Pd 变成 P’ -> aP'|bP'|cP'|dP'|ε
            auto *new_res_productions = new std::vector<Productions *>();
            for (Productions *productions: left_eliminate_productions) {
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


std::vector<std::vector<int>> subExtraction(CFG *cfg, int start, int newId, std::vector<int> &factor,
                                            std::vector<std::vector<int>> &factoring_productions) {
    // <start> -> <maxFactor><newId>
    std::vector<int> new_production(factor);    // 新产生式右边
    new_production.push_back(newId);
    cfg->products.insert(Productions(start, new_production));
    cfg->nonter.insert(newId);

    // <newId> -> XXX | XX
    std::vector<std::vector<int>> newFactoring_prod;
    for (auto &prod: factoring_productions) {
        prod.erase(prod.begin(), prod.begin() + (int) factor.size());
        //这个因子刚好就是一整个产生式
        if (prod.empty()) {
            std::vector<int> emptyProd{CFG::EMPTY_ID};
            cfg->products.insert(Productions(newId, emptyProd));
        } else {
            newFactoring_prod.push_back(prod);
        }
    }
    return newFactoring_prod;
}

void partPrefixExtraction(CFG *cfg, int start, const std::vector<std::vector<int>> &factoring_productions) {
    std::vector<std::vector<int>> fact_prod(factoring_productions);
    while (true) {
        //提取有公共前缀的部分产生式，取尽量包含更多产生式的公共前缀
        std::vector<std::vector<int>> hasFactor_prod;
        std::vector<int> hasFactor;    //该公共前缀
        std::vector<int> hasFactorPos; //该公共前缀在待处理产生式中的下标位置
        for (const auto &prod1: fact_prod) {
            for (const auto &prod2: fact_prod) {
                if (prod1 == prod2) continue;
                int len = prod1.size() <= prod2.size() ? (int) prod2.size() : (int) prod1.size();
                int idx = 0;
                while (idx < len) {
                    if (prod1[idx] != prod2[idx]) break;
                    idx++;
                }
                if (idx == 0) continue;
                //部分公共前缀
                std::vector<int> partFactor(prod1.begin(), prod1.begin() + idx);
                //包含该前缀的产生式
                std::vector<std::vector<int>> partProd;
                std::vector<int> factorPos; //这些产生式在待处理产生式中的下标位置
                for (int i = 0; i < fact_prod.size(); i++) {
                    std::vector<int> prod3 = fact_prod[i];
                    if (prod3.size() < partFactor.size()) continue;
                    int isOK = true;
                    int pos = 0;
                    while (pos < partFactor.size()) {
                        if (prod3[pos] != partFactor[pos]) {
                            isOK = false;
                            break;
                        }
                        pos++;
                    }
                    if (isOK) {
                        partProd.push_back(prod3);
                        factorPos.push_back(i);
                    }
                }
                if (partProd.size() > hasFactor_prod.size()) {
                    hasFactor_prod.swap(partProd);
                    hasFactor.swap(partFactor);
                    hasFactorPos.swap(factorPos);
                }
            }
        }
        //找不到则直接退出
        if (hasFactor.empty()) {
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
        for (int it: hasFactorPos) {
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
        if (factoring_productions.empty()) continue;
        // 寻找公共前缀
        std::vector<int> maxFactor = *factoring_productions.begin();
        for (auto prod: factoring_productions) {
            int idx = 0;
            while (idx < maxFactor.size() && idx < prod.size()) {
                if (prod[idx] != maxFactor[idx]) break;
                idx++;
            }
            maxFactor.erase(maxFactor.begin() + idx, maxFactor.end());
        }
        //提取公共前缀
        if (!maxFactor.empty()) {
            int newId = CFG::newId();
            // <start> -> <maxFactor><newId>
            // <newId> -> XXX | XX
            std::vector<std::vector<int>> newFactoring_prod = subExtraction(opg, start, newId, maxFactor,
                                                                            factoring_productions);
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

void dfsFirst(std::vector<Productions *> &items, std::unordered_map<int, std::vector<int>> *ans,
              std::unordered_map<int, std::vector<Productions *>> *map, const CFG &cfg) {
    auto ans_vector = new std::vector<int>;
    for (auto item: items) {
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
            for (auto v_item: v->second) {
                ans_vector->push_back(v_item);
            }
            // 终结符
        } else {
            ans_vector->push_back(rightFirst);
        }
    }
    ans->insert({items[0]->start, *ans_vector});
}

// TODO:free 内存
void LL::FirstSetSolver(CFG &cfg) {
    auto m = format(cfg);
    auto ans = new std::unordered_map<int, std::vector<int>>;
    for (auto item: *m) {
        dfsFirst(item.second, ans, m, cfg);
    }
    auto set = new std::unordered_set<SubSet, SubSet::SubSetHasher>;
    for (auto item: *ans) {
        auto sub_set = new SubSet;
        sub_set->symbol = item.first;
        for (auto item_int: item.second) {
            sub_set->st.insert(item_int);
        }
        set->insert(*sub_set);
    }
    cfg.firstSet = *set;
}

int is_change(std::unordered_map<int, std::unordered_set<int>> &fol_m, int symbol, int ist) {
    if (fol_m.find(symbol) == fol_m.end()) {//键不存在
        fol_m[symbol].insert(ist);
        return true;
    } else {
        // 如果键存在，直接插入值
        if (fol_m[symbol].find(ist) == fol_m[symbol].end()) {//判断值不存在
            fol_m[symbol].insert(ist);
            return true;
        } else { return false; }
    }
}

std::unordered_set<int> getSetBySymbol(std::unordered_set<SubSet, SubSet::SubSetHasher> &cfgSet, int symbol) {
    std::unordered_set<int> Set;
    for (const auto &subset: cfgSet) {
        if (subset.symbol == symbol) {
            Set.insert(subset.st.begin(), subset.st.end());
        }
    }
    return Set;
}

void LL::FollowSetSolver(CFG &cfg) {
    std::unordered_map<int, std::unordered_set<int>> fol_m;
    //规则一：加入#号

    bool change = is_change(fol_m, cfg.start, CFG::Terminal_ID);
    bool if_change = true;
    while (if_change) {
        if_change = false;
        for (const auto &prod: cfg.products) {
            for (int i = 0; i < prod.grammar.size() - 1; i++) {
                int s = prod.grammar[i];
                int next_s = prod.grammar[i + 1];
                if (cfg.nonter.find(s) != cfg.nonter.end() && i != prod.grammar.size()) {//规则二：找到一个非终结符
                    if (cfg.ter.find(next_s) != cfg.ter.end())//非终结符后的终结符
                    {
                        change = is_change(fol_m, s, next_s);
                        if (change) { if_change = true; }
                    }
                    for (int j = 1; j + i < prod.grammar.size(); j++) {
                        next_s = prod.grammar[i + j];
                        if (cfg.nonter.find(next_s) != cfg.nonter.end())//非终结符后的非终结符
                        {
                            std::unordered_set<int> fir_set;
                            fir_set = getSetBySymbol(cfg.firstSet, next_s);
                            for (auto const id: fir_set) {
                                if (id != CFG::EMPTY_ID) {
                                    change = is_change(fol_m, s, id);
                                    if (change) { if_change = true; }
                                }
                            }
                            if (fir_set.find(CFG::EMPTY_ID) == fir_set.end()) {
                                break;
                            }//first集除了空全给
                        } else {
                            change = is_change(fol_m, s, next_s);
                            if (change) { if_change = true; }
                            break;
                        }
                    }
                }

            }

            int las = prod.grammar.back();

            if (cfg.nonter.find(las) != cfg.nonter.end()) {//规则三：非终结符的follow给非终结符

                std::unordered_set<int> fol_set;
                std::unordered_set<int> fir_set;

                fol_set = getSetBySymbol(cfg.followSet, prod.start);

                for (auto const id: fol_set) {
                    change = is_change(fol_m, las, id);
                    if (change) { if_change = true; }
                }

                for (int sj = 1; sj <= prod.grammar.size(); sj++) {//非终结符的first集可能存在空
                    int pr_las = prod.grammar[prod.grammar.size() - sj];
                    if (cfg.nonter.find(pr_las) != cfg.nonter.end()) {
                        fir_set = getSetBySymbol(cfg.firstSet, las);
                        if (fir_set.find(CFG::EMPTY_ID) != fir_set.end()) {
                            for (auto const id: fol_set) {
                                change = is_change(fol_m, pr_las, id);
                                if (change) { if_change = true; }
                            }
                            las = pr_las;
                        } else { break; }
                    } else { break; }

                }
                //follow集全给
                if (if_change) {
                    for (auto &it: fol_m) {
                        SubSet Follow{it.first, it.second};
                        cfg.followSet.insert(Follow);
                    }
                }
            }
        }
    }
    cfg.followSet.clear();
    for (auto &it: fol_m) {
        SubSet Follow{it.first, it.second};
        cfg.followSet.insert(Follow);
    }
}

bool LL::isLLFoundation(const CFG &cfg) {
    auto m_p = format(cfg);
    std::unordered_map<int, std::unordered_set<int>> firstMap, followMap;
    for (const auto &f: cfg.firstSet) {
        firstMap[f.symbol] = f.st;
    }
    for (const auto &f: cfg.followSet) {
        followMap[f.symbol] = f.st;
    }
    for (const auto &f: cfg.firstSet) {
        int sp = f.symbol;
        auto prodSet = m_p->find(sp)->second;
        std::unordered_set<int> st = f.st;  //sp的first集合
        for (int t: st) {
            if (t == CFG::EMPTY_ID) {    //有空则检查follow集合
                auto follow = followMap.find(sp)->second;
                for (int fo: follow) {
                    if (LL::analysisTable[sp].find(fo) != LL::analysisTable[sp].end()) {
                        return false;
                    }
                    std::vector<int> a = {CFG::EMPTY_ID};
                    LL::analysisTable[sp][fo] = a;
                }
                continue;
            }
            for (const Productions *prod: prodSet) {
                if (firstMap[prod->grammar[0]].find(t) != firstMap[prod->grammar[0]].end() || t == prod->grammar[0]) {
                    if (LL::analysisTable[sp].find(t) != LL::analysisTable[sp].end()) {
                        return false;
                    }
                    std::vector<int> a = prod->grammar;
                    LL::analysisTable[sp][t] = a;
                }
            }
        }
    }
    return true;
}
