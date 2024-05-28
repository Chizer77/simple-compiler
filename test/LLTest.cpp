#include <cstdio>
#include <map>
#include <string>
#include <iostream>
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

    // F->i
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
    CFG *cfg = new CFG();
    int S = CFG::newId();
    int A = CFG::newId();
    int B = CFG::newId();
    int a = CFG::newId();
    int b = CFG::newId();
    int c = CFG::newId();
    int d = CFG::newId();
    cfg->start = S;
    cfg->nonter = {S, A, B};
    cfg->ter = {a, b, c, d};
    // S->Ab|Abc|bc
    std::vector<int> prod = {A, b};
    cfg->products.insert(Productions(S, prod));
    prod.clear();
    prod = {A, b, c};
    cfg->products.insert(Productions(S, prod));
    prod.clear();
    prod = {b, c};
    cfg->products.insert(Productions(S, prod));
    prod.clear();
    // A->Bcd|Bd|Bc|ca
    prod = {B, c, d};
    cfg->products.insert(Productions(A, prod));
    prod.clear();
    prod = {B, d};
    cfg->products.insert(Productions(A, prod));
    prod.clear();
    prod = {B, c};
    cfg->products.insert(Productions(A, prod));
    prod.clear();
    prod = {c, a};
    cfg->products.insert(Productions(A, prod));
    CFG *res = LL::LeftFactorExtraction(*cfg);
}

void LLTest::LeftFactorExtractionTest02() {
    CFG *cfg = new CFG();
    int S = CFG::newId();
    int A = CFG::newId();
    int B = CFG::newId();
    int a = CFG::newId();
    int b = CFG::newId();
    int c = CFG::newId();
    int d = CFG::newId();
    int e = CFG::newId();
    cfg->start = S;
    cfg->nonter = {S, A, B};
    cfg->ter = {a, b, c, d, e};
    // S->aA | aB | abc
    std::vector<int> prod = {a, A};
    cfg->products.insert(Productions(S, prod));
    prod.clear();
    prod = {a, B};
    cfg->products.insert(Productions(S, prod));
    prod.clear();
    prod = {a, b, c};
    cfg->products.insert(Productions(S, prod));
    prod.clear();
    // A -> b | c
    prod = {b};
    cfg->products.insert(Productions(A, prod));
    prod.clear();
    prod = {c};
    cfg->products.insert(Productions(A, prod));
    prod.clear();
    //B -> d | e
    prod = {d};
    cfg->products.insert(Productions(A, prod));
    prod.clear();
    prod = {e};
    cfg->products.insert(Productions(A, prod));
    CFG *res = LL::LeftFactorExtraction(*cfg);
}

void LLTest::test() {

    std::map<int, std::string> m;
    CFG *cfg = new CFG();
    int S = CFG::newId();m.insert({S, "S"});
    int A = CFG::newId();m.insert({A, "A"});
    int B = CFG::newId();m.insert({B, "B"});
    int a = CFG::newId();m.insert({a, "a"});
    int b = CFG::newId();m.insert({b, "b"});
    int c = CFG::newId();m.insert({c, "c"});
    m.insert({CFG::UNION_ID, "|"});
    m.insert({CFG::EMPTY_ID, "空集"});
    cfg->start = S;
    cfg->nonter = {A, B, S};
    cfg->ter = {a, b, c};

    // S -> Bc
    std::vector<int> prod = {B, c};
    cfg->products.insert(Productions(S, prod));
    prod.clear();

    // S->Ab
    prod = {A, b};
    cfg->products.insert(Productions(S, prod));
    prod.clear();

    // B -> Ab
    prod = {A, b};
    cfg->products.insert(Productions(B, prod));
    prod.clear();

    // A -> Sa|b
    prod = {S, a, CFG::UNION_ID, b};
    cfg->products.insert(Productions(A, prod));
    prod.clear();

    auto cfg_new = LL::LeftRecurElimination(*cfg);

}

void LLTest::FirstSetSolverTest01(){
    std::map<int, std::string> m;
    CFG *cfg = new CFG();
    int E = CFG::newId();m.insert({E, "E"});
    int A= CFG::newId();m.insert({A, "A"});
    int B = CFG::newId();m.insert({B, "B"});
    int C = CFG::newId();m.insert({C, "C"});
    int D = CFG::newId();m.insert({D, "D"});
    int a = CFG::newId();m.insert({a, "a"});
    int b = CFG::newId();m.insert({b, "b"});
    int c = CFG::newId();m.insert({c, "c"});
    int d = CFG::newId();m.insert({d, "d"});
    m.insert({CFG::UNION_ID, "|"});
    m.insert({CFG::EMPTY_ID, "空集"});
    cfg->start = E;
    cfg->nonter = {E, A, B, C, D};
    cfg->ter = {a, b, c, d};

    // E->CA
    std::vector<int> p_list_S = {C, A};
    auto *p_E = new Productions(E, p_list_S);

    // A->dC|空集
    std::vector<int> p_list_A = {d, C, CFG::UNION_ID, CFG::EMPTY_ID};
    auto *p_A = new Productions(A, p_list_A);

    // C->DB
    std::vector<int> p_list_C = {D, B};
    auto *p_C = new Productions(C, p_list_C);

    // B->aDB|空集
    std::vector<int> p_list_B = {a, D, B, CFG::UNION_ID, CFG::EMPTY_ID};
    auto *p_B = new Productions(B, p_list_B);

    // D->bEb|c
    std::vector<int> p_list_D = {b, E, b, CFG::UNION_ID, c};
    auto *p_D = new Productions(D, p_list_D);

    std::unordered_set<Productions, Productions::ProductionsHasher> products;
    products.insert(*p_E);
    products.insert(*p_A);
    products.insert(*p_B);
    products.insert(*p_C);
    products.insert(*p_D);

    cfg->products = products;

    LL::FirstSetSolver(*cfg);
    printf("\n");
    for (const auto& item : cfg->firstSet) {
        printf("first(%s) = ", m.find(item.symbol)->second.c_str());
        for (auto item_v : item.st) {
            printf("%s,", m.find(item_v)->second.c_str());
        }
        printf("\n");
    }
}


void LLTest::FollowSetSolverTest01(){
    CFG *cfg = new CFG();
    int E = CFG::newId();
    int A = CFG::newId();
    int B = CFG::newId();
    int C = CFG::newId();
    int D = CFG::newId();
    int a = CFG::newId();
    int b = CFG::newId();
    int c = CFG::newId();
    int d = CFG::newId();

    cfg->start = E;
    cfg->nonter = {E, A, B, C, D};
    cfg->ter = {a, b, c, d};

    // E->CA
    std::vector<int> p_list_S = {C, A};
    auto *p_E = new Productions(E, p_list_S);

    // A->dC|空集
    std::vector<int> p_list_A = {d, C};
    auto *p_A = new Productions(A, p_list_A);

    // A->空集
    std::vector<int> p_list_A1 = {CFG::EMPTY_ID};
    auto *p_A1 = new Productions(A, p_list_A1);

    // C->DB
    std::vector<int> p_list_C = {D, B};
    auto *p_C = new Productions(C, p_list_C);

    // B->aDB
    std::vector<int> p_list_B = {a, D, B};
    auto *p_B = new Productions(B, p_list_B);

    // B->空集
    std::vector<int> p_list_B1 = {CFG::EMPTY_ID};
    auto *p_B1 = new Productions(B, p_list_B1);

    // D->bEb
    std::vector<int> p_list_D = {b, E, b};
    auto *p_D = new Productions(D, p_list_D);

    // D->c
    std::vector<int> p_list_D1 = {c};
    auto *p_D1 = new Productions(D, p_list_D1);

    std::unordered_set<Productions, Productions::ProductionsHasher> products;
    products.insert(*p_E);
    products.insert(*p_A);
    products.insert(*p_A1);
    products.insert(*p_B);
    products.insert(*p_B1);
    products.insert(*p_C);
    products.insert(*p_D);
    products.insert(*p_D1);

    cfg->products = products;

    std::unordered_set<SubSet, SubSet::SubSetHasher> f_Set;

    // E first
    std::unordered_set<int> list_E = {c,b};
    auto *E_f = new SubSet{E, list_E};
    // D first
    std::unordered_set<int> list_D = {c,b};
    auto *D_f = new SubSet{D, list_D};
    // A first
    std::unordered_set<int> list_A = {d,CFG::EMPTY_ID};
    auto *A_f = new SubSet{A, list_A};
    // B first
    std::unordered_set<int> list_B = {a,CFG::EMPTY_ID};
    auto *B_f = new SubSet{B, list_B};
    // C first
    std::unordered_set<int> list_C = {c,b};
    auto *C_f = new SubSet{C, list_C};


    f_Set.insert(*E_f);
    f_Set.insert(*D_f);
    f_Set.insert(*A_f);
    f_Set.insert(*B_f);
    f_Set.insert(*C_f);
    cfg->firstSet = f_Set;

    LL::FollowSetSolver(*cfg);

    std::cout << std::endl;
    std::cout << "Follow集合：" << std::endl;

    for(const SubSet& subset : cfg->followSet) {
        std::cout << "符号: " << subset.symbol << ", ";
        std::cout << "Follow集: ";
        for(int s : subset.st) {
            std::cout << s << " ";
        }
        std::cout << std::endl;
    }
}
