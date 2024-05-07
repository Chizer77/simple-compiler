#include <cstdio>
#include <map>
#include <string>
#include "LLTest.h"
#include "frontend/LL.h"

void LLTest::LeftRecurEliminationTest01() {
    std::map<int, std::string> m;
    CFG *cfg = new CFG();
    int S = CFG::newId();m.insert({S, "S"});
    int A = CFG::newId();m.insert({A, "A"});
    int a = CFG::newId();m.insert({a, "a"});
    int b = CFG::newId();m.insert({b, "b"});
    int c = CFG::newId();m.insert({c, "c"});
    int d = CFG::newId();m.insert({d, "d"});
    m.insert({CFG::UNION_ID, "|"});
    cfg->start = S;
    cfg->nonter = {S, A};
    cfg->ter = {a, b, c, d};

    // S->Aa|b
    std::vector<int> p_list_S = {A, a, CFG::UNION_ID, b};
    auto *p_S = new Productions(S, p_list_S);

    // A->Ac|Sd|a
    std::vector<int> p_list_A = {A, c, CFG::UNION_ID, d, CFG::UNION_ID, a};
    auto *p_A = new Productions(A, p_list_A);

    std::unordered_set<Productions, Productions::ProductionsHasher> products;
    products.insert(*p_A);
    products.insert(*p_S);
    cfg->products = products;
    auto cfg_new = LL::LeftRecurElimination(*cfg);
    printf("\n非终结符：\n");
    for (int i : cfg_new->nonter) {
        auto iter = m.find(i);
        if (iter != m.end()) {
            printf("%s ", iter->second.c_str());
        } else {
            printf("%d ", iter->first);
        }
    }
    printf("\n终结符：\n");
    for (int i : cfg_new->ter) {
        auto iter = m.find(i);
        printf("%s ", iter->second.c_str());
    }
    printf("\nProductions：\n");
    for (const Productions& p : cfg_new->products) {
        auto iter = m.find(p.start);
        if (iter != m.end()) {
            printf("%s ", iter->second.c_str());
        } else {
            printf("%d ", iter->first);
        }
        printf("->");
        for (int i : p.grammar) {
            auto iter_2 = m.find(i);
            if (iter_2 != m.end()) {
                printf("%s ", iter_2->second.c_str());
            } else  {
                printf("%d ", iter_2->first);
            }
        }
        printf("\n");
    }
}

void LLTest::LeftFactorExtractionTest01() {

}
