#include "iostream"
#include "util/Init.h"
#include "NfaTest.h"
#include "DfaTest.h"
#include <ctime>

void log(void (*func)()) {
    timespec start{}, end{};
    clock_gettime(CLOCK_MONOTONIC, &start);
    (*func)();
    clock_gettime(CLOCK_MONOTONIC, &end);
    double duration = ((double)(end.tv_nsec-start.tv_nsec) / ((double) 1e9) + (double)(end.tv_sec-start.tv_sec)) * 1000;
    std::cout << "Completed in " << duration << "ms." << std::endl;
    std::cout << "--------------------------" << std::endl;
}

int main() {
    std::cout << "hello world" << std::endl;
    std::cout << "NfaTest runs..." << std::endl;
    log(NfaTest::run);
    std::cout << "DfaTest runs..." << std::endl;
    log(DfaTest::run);
}