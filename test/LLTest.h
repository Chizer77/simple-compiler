#ifndef SIMPLE_COMPILER_LLTEST_H
#define SIMPLE_COMPILER_LLTEST_H


class LLTest {
public:
    static void LeftRecurEliminationTest01();

    static void LeftFactorExtractionTest01();

    static void run() {
        LeftRecurEliminationTest01();
        LeftFactorExtractionTest01();
    }
};


#endif //SIMPLE_COMPILER_LLTEST_H
