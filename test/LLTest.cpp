#include <cstdio>
#include <map>
#include <string>
#include <iostream>
#include "LLTest.h"
#include "frontend/LL.h"
#include "util/FileIO.h"

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

void LLTest::LeftRecurEliminationInputTest(const std::string& filePath) {
    std::string s = FileIO::read(filePath.c_str());
    CFG *cfg = new CFG();
    std::unordered_map<char, int> mp;   //数字映射
    std::unordered_map<int, char> remp;
    int newId = CFG::newId();
    cfg->start = newId;
    mp[s[0]] = newId;
    remp[newId] = s[0];
    cfg->nonter.insert(cfg->start);
    int idx = 2;
    while(idx < s.size()) {
        std::string line;
        while(idx < s.size() && s[idx] != '\n') {
            line += s[idx++];
        }
        idx++;
        Productions prod;
        if(mp.find(line[0]) != mp.end()) {
            prod.start = mp[line[0]];
            cfg->nonter.insert(prod.start);
        }else {
            newId = CFG::newId();
            mp[line[0]] = newId;
            remp[newId] = line[0];
            prod.start = newId;
            cfg->nonter.insert(prod.start);
        }
        int id = 2;
        while (id < line.size()) {
            if(line[id] == ' ') {
                prod.grammar.push_back(CFG::UNION_ID);
            }else if(line[id] == '$') {
                prod.grammar.push_back(CFG::EMPTY_ID);
                mp[line[id]] = CFG::EMPTY_ID;
                remp[CFG::EMPTY_ID] = line[id];
            }else {
                int t;
                if(mp.find(line[id]) == mp.end()) {
                    t = CFG::newId();
                    mp[line[id]] = t;
                    remp[t] = line[id];
                } else {
                    t = mp[line[id]];
                }
                prod.grammar.push_back(t);
                if(line[id] >= 'a' && line[id] <= 'z') {
                    cfg->ter.insert(t);
                }else if(line[id] >= 'A' && line[id] <= 'Z') {
                    cfg->nonter.insert(t);
                }
            }
            id++;
        }
        cfg->products.insert(prod);
    }
    LL::Split(*cfg);
    //S
    //S Aa b
    //A Ac Sd $

    //S
    //S Ab Abc bc
    //A Bcd Bd Bc ca
    //B $
    auto cfg_new = LL::LeftRecurElimination(*cfg);
    LL::Split(*cfg_new);
    cfg_new = LL::LeftFactorExtraction(*cfg_new);
    //A
    //A Aaa Aac ccc
//    LL::FirstSetSolver(*cfg);
//    LL::FollowSetSolver(*cfg);
    std::string a;
    a += remp[cfg_new->start];
    a += "\n";
    for(const auto& p: cfg_new->products) {
        if(remp.find(p.start) == remp.end()) {
            a += std::to_string(p.start);
        }else a += remp[p.start];
        a += " ";
        for(int g: p.grammar) {
            if(g == CFG::UNION_ID) {
                a += "|";
            }else if(g == CFG::EMPTY_ID) {
                a += "$";
            }
            else if(remp.find(g) == remp.end()) {
                a += "\"" + std::to_string(g) + "\"";
            }else a += remp[g];
        }
        a += "\n";
    }
    FileIO::write("../output/ll.txt", a);
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

    // A->dC
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

    if(!LL::isLLFoundation(*cfg)) std::cout<<"Error!\n";

    for(const auto& it: LL::analysisTable) {
        std::cout << it.first << "\n";
        for(const auto& fo: LL::analysisTable[it.first]) {
            std::cout << fo.first << "     ";
            for(int k: fo.second) {
                std::cout << k << ' ';
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }
}




void LLTest::FollowSetSolverTest02(){
    CFG *cfg = new CFG();
    int S = CFG::newId();
    int T = CFG::newId();
    int R = CFG::newId();
    int a = CFG::newId();
    int b = CFG::newId();
    int e = CFG::newId();

    cfg->start = S;
    cfg->nonter = {S, T, R};
    cfg->ter = {a, b, e};

    // S->Te
    std::vector<int> p_list_S = {T,e};
    auto *p_S = new Productions(S, p_list_S);

    // S->STRb
    std::vector<int> p_list_S1 = {S,T,R,b};
    auto *p_S1 = new Productions(S, p_list_S1);

    // T->空
    std::vector<int> p_list_T1 = {CFG::EMPTY_ID};
    auto *p_T1 = new Productions(T, p_list_T1);

    // T->R
    std::vector<int> p_list_T = {R};
    auto *p_T = new Productions(T, p_list_T);

    // R->a
    std::vector<int> p_list_R = {a};
    auto *p_R = new Productions(R, p_list_R);

    // R->空集
    std::vector<int> p_list_R1 = {CFG::EMPTY_ID};
    auto *p_R1 = new Productions(R, p_list_R1);

    // R->STT
    std::vector<int> p_list_R2 = {S,T,T};
    auto *p_R2 = new Productions(R, p_list_R2);


    std::unordered_set<Productions, Productions::ProductionsHasher> products;
    products.insert(*p_S);
    products.insert(*p_S1);
    products.insert(*p_T);
    products.insert(*p_T1);
    products.insert(*p_R);
    products.insert(*p_R1);
    products.insert(*p_R2);

    cfg->products = products;

    std::unordered_set<SubSet, SubSet::SubSetHasher> f_Set;

    // S first
    std::unordered_set<int> list_S = {e};
    auto *S_f = new SubSet{S, list_S};
    // T first
    std::unordered_set<int> list_T = {a,e,CFG::EMPTY_ID};
    auto *T_f = new SubSet{T, list_T};
    // R first
    std::unordered_set<int> list_R = {a,e,CFG::EMPTY_ID};
    auto *R_f = new SubSet{R, list_R};


    f_Set.insert(*S_f);
    f_Set.insert(*T_f);
    f_Set.insert(*R_f);
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

void LLTest::FollowSetSolverTest03(){
    CFG *cfg = new CFG();
    int S = CFG::newId();
    int T = CFG::newId();
    int R = CFG::newId();
    int D = CFG::newId();
    int a = CFG::newId();
    int b = CFG::newId();
    int d = CFG::newId();
    int e = CFG::newId();

    cfg->start = S;
    cfg->nonter = {S, T, R, D};
    cfg->ter = {a, b, d, e};

    // S->eT
    std::vector<int> p_list_S = {e,T};
    auto *p_S = new Productions(S, p_list_S);

    // S->RT
    std::vector<int> p_list_S1 = {R,T};
    auto *p_S1 = new Productions(S, p_list_S1);

    // T->空集
    std::vector<int> p_list_T1 = {CFG::EMPTY_ID};
    auto *p_T1 = new Productions(T, p_list_T1);

    // T->DR
    std::vector<int> p_list_T = {D,R};
    auto *p_T = new Productions(T, p_list_T);

    // R->dR
    std::vector<int> p_list_R = {d,R};
    auto *p_R = new Productions(R, p_list_R);

    // R->空集
    std::vector<int> p_list_R1 = {CFG::EMPTY_ID};
    auto *p_R1 = new Productions(R, p_list_R1);

    // D->a
    std::vector<int> p_list_D = {a};
    auto *p_D = new Productions(D, p_list_D);

    // D->bd
    std::vector<int> p_list_D1 = {b,d};
    auto *p_D1 = new Productions(D, p_list_D1);


    std::unordered_set<Productions, Productions::ProductionsHasher> products;
    products.insert(*p_S);
    products.insert(*p_S1);
    products.insert(*p_T);
    products.insert(*p_T1);
    products.insert(*p_R);
    products.insert(*p_R1);
    products.insert(*p_D);
    products.insert(*p_D1);

    cfg->products = products;

    std::unordered_set<SubSet, SubSet::SubSetHasher> f_Set;

    // S first
    std::unordered_set<int> list_S = {a,b,d,e,CFG::EMPTY_ID};
    auto *S_f = new SubSet{S, list_S};
    // T first
    std::unordered_set<int> list_T = {a,b,CFG::EMPTY_ID};
    auto *T_f = new SubSet{T, list_T};
    // R first
    std::unordered_set<int> list_R = {d,CFG::EMPTY_ID};
    auto *R_f = new SubSet{R, list_R};
    // D first
    std::unordered_set<int> list_D = {a,b};
    auto *D_f = new SubSet{D, list_D};

    f_Set.insert(*S_f);
    f_Set.insert(*T_f);
    f_Set.insert(*R_f);
    f_Set.insert(*D_f);
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