#include <iostream>
#include <chrono>
#include <unordered_map>
#include "../include/util/Init.h"
#include "TestCenter.h"
#include "NfaTest.h"
#include "DfaTest.h"
#include "LLTest.h"
#include "frontend/Dfa.h"
#include "frontend/Lexer.h"

void log(void (*func)()) {
    auto start = std::chrono::high_resolution_clock::now();
    (*func)();
    auto end = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration<double>(end - start).count() * 1000;
    std::cout << "Completed in " << duration << "ms." << std::endl;
    std::cout << "--------------------------" << std::endl;
}

int main() {
//    std::cout << "NfaTest runs..." << std::endl;
//    log(NfaTest::run);
//    std::cout << "DfaTest runs..." << std::endl;
//    log(DfaTest::run);
    std::cout << "LLTest runs..." << std::endl;
    log(LLTest::run);

}