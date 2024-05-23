#include <iostream>
#include <chrono>
#include "../include/util/Init.h"
#include "TestCenter.h"
#include "NfaTest.h"
#include "DfaTest.h"
#include "LLTest.h"
#include "frontend/Dfa.h"

void log(void (*func)()) {
    auto start = std::chrono::high_resolution_clock::now();
    (*func)();
    auto end = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration<double>(end - start).count() * 1000;
    std::cout << "Completed in " << duration << "ms." << std::endl;
    std::cout << "--------------------------" << std::endl;
}

std::string format(std::string s) {
    std::string ans;
    int idx = 0;
    while(idx < s.size()) {
        if(s[idx] == '|') {
            ans += Nfa::UNION_STATE;
        }else if(s[idx] == '*') {
            ans += Nfa::KLEENE_STATE;
        }else if(s[idx] == '.') {
            ans += Nfa::CONNECTION_STATE;
        }else if(s[idx] == '(') {
            ans += Nfa::LPARENT_STATE;
        }else if(s[idx] == ')'){
            ans += Nfa::RPARENT_STATE;
        }else {
            ans += s[idx];
        }
        idx++;
    }
    return ans;
}

void out(Graph *nfa) {
    std::cout << "S: \n";
    for(auto it: nfa->s) {
        std::cout << it << ' ';
    }
    std::cout << "\n";
    std::cout << "S0: \n" << nfa->s0 << std::endl;
    std::cout << "Targets: \n";
    for(auto it: nfa->target) {
        std::cout << it << ' ';
    }
    std::cout << "\n";
    std::cout << "Alpha: \n";
    for(auto it: nfa->alpha) {
        if(it == Nfa::EMPTY_STATE) std::cout << "ε" << ' ';
        else std::cout << it << ' ';
    }
    std::cout << "\n";
    std::cout << "Edges: \n";
    for(auto it: nfa->edges) {
        std::string c;
        c = it.alpha;
        if(it.alpha == Nfa::EMPTY_STATE) c = "ε";
        std::cout << it.start << "--" << c << "-->" << it.target << '\n';
    }
    std::cout << "\n\n";
}
int main() {
//    std::cout << "NfaTest runs..." << std::endl;
//    log(NfaTest::run);
//    std::cout << "DfaTest runs..." << std::endl;
//    log(DfaTest::run);
//    std::cout << "LLTest runs..." << std::endl;
//    log(LLTest::run);
    std::string s;
    while(true) {
        std::cout << "input tokens: \n";
        std::cin >> s;
        if(s == "-1") break;
        s = format(s);
        Nfa *nfa = Nfa::Generation(s);
        std::cout << "The NFA: \n";
        out(nfa);
        std::cout << "Nfa2Dfa: \n";
        Dfa *dfa = Dfa::Nfa2Dfa(*nfa);
        out(dfa);
        std::cout << "DfaMinimize: \n";
        dfa = Dfa::DfaMinimize(*dfa);
        out(dfa);
    }
}