#include "iostream"
#include "include/frontend/nfa/Nfa.h"
#include "include/util/Init.h"

int main() {
    std::cout << "hello world" << std::endl;
    std::string expression;
    std::cin >> expression;
    Nfa a = Nfa::NfaGeneration(expression);
    std::string e2;
    std::cin >> e2;
    Nfa b = Nfa::NfaGeneration(e2);
    std::cout << "ok" << std::endl;
}