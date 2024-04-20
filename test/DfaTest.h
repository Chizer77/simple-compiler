#ifndef SIMPLE_COMPILER_DFATEST_H
#define SIMPLE_COMPILER_DFATEST_H


class DfaTest {
public:
    static void DfaMinimizeTest01();

    static void DfaMinimizeTest02();

    static void Nfa2DfaTest01();

    static  void Nfa2DfaTest02();

    static  void Nfa2DfaTest03();

    static  void DfaMinimizeTest001();

    static  void DfaMinimizeTest002();

    static  void DfaMinimizeTest003();

    static void run() {
        DfaMinimizeTest01();
        DfaMinimizeTest02();
        Nfa2DfaTest01();
        Nfa2DfaTest02();
        Nfa2DfaTest03();
        DfaMinimizeTest001();
        DfaMinimizeTest002();
        DfaMinimizeTest003();
    }
};


#endif //SIMPLE_COMPILER_DFATEST_H
