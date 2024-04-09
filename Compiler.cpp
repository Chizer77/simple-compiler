#include "iostream"
#include "util/Init.h"
#include "NfaTest.h"
#include "DfaTest.h"


int main() {
    std::cout << "hello world" << std::endl;

    NfaTest::run();
    DfaTest::run();
}