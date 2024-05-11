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
    m.insert({CFG::EMPTY_ID, "空集"});
    cfg->start = S;
    cfg->nonter = {S, A};
    cfg->ter = {a, b, c, d};

    // S->Aa|b
    std::vector<int> p_list_S = {A, a, CFG::UNION_ID, b};
    auto *p_S = new Productions(S, p_list_S);

    // A->Ac|Sd
    std::vector<int> p_list_A = {A, c, CFG::UNION_ID, S, d};
    auto *p_A = new Productions(A, p_list_A);

    std::unordered_set<Productions, Productions::ProductionsHasher> products;
    products.insert(*p_S);
    products.insert(*p_A);
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
        printf("-> ");
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


void LLTest::LeftRecurEliminationTest02() {//发现bug:如果要添加多个左递归，只会new一个新的id

    std::map<int, std::string> m;
    CFG *cfg = new CFG();
    int E = CFG::newId();m.insert({E, "E"});
    int T = CFG::newId();m.insert({T, "T"});
    int F = CFG::newId();m.insert({F, "F"});
    int j = CFG::newId();m.insert({j, "j"});
    int c = CFG::newId();m.insert({c, "c"});
    int id = CFG::newId();m.insert({id, "id"});
    m.insert({CFG::UNION_ID, "|"});
    m.insert({CFG::EMPTY_ID, "空集"});
    cfg->start = E;
    cfg->nonter = {E, T, F};
    cfg->ter = {j, c, id};

    // E->EjT|T
    std::vector<int> p_list_E = {E, j, T, CFG::UNION_ID, T};
    auto *p_E = new Productions(E, p_list_E);

    // T->TcF|F
    std::vector<int> p_list_T = {T, c, F, CFG::UNION_ID, F};
    auto *p_T = new Productions(T, p_list_T);

    // T->i
    std::vector<int> p_list_F = {id};
    auto *p_F = new Productions(F, p_list_F);

    std::unordered_set<Productions, Productions::ProductionsHasher> products;
    products.insert(*p_E);
    products.insert(*p_T);
    products.insert(*p_F);
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
        printf("-> ");
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
