#ifndef SIMPLE_COMPILER_DFATEST_H
#define SIMPLE_COMPILER_DFATEST_H


class DfaTest {
public:
    static void DfaMinimizeTest01();

    static void DfaMinimizeTest02();

    static void Nfa2DfaTest01();

    static void run() {
        DfaMinimizeTest01();
        DfaMinimizeTest02();
        Nfa2DfaTest01();
    }
};


#endif //SIMPLE_COMPILER_DFATEST_H
