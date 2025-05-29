#ifndef SIMPLE_COMPILER_LLTEST_H
#define SIMPLE_COMPILER_LLTEST_H


class LLTest {
public:
    static void LeftRecurEliminationTest01();

    static void LeftRecurEliminationTest02();

    static void LeftFactorExtractionTest01();

    static void LeftFactorExtractionTest02();

    static void FirstSetSolverTest01();

    static void FollowSetSolverTest01();

    static void FollowSetSolverTest02();

    static void FollowSetSolverTest03();

    /**
     * 左递归文本输入
     * @param filePath
     */
    static void LeftRecurEliminationInputTest(const std::string &filePath);

    static void run() {
//        LeftRecurEliminationInputTest("../input/grammar.txt");
        LeftRecurEliminationTest01();
        LeftRecurEliminationTest02();
        LeftFactorExtractionTest01();
        LeftFactorExtractionTest02();
        FirstSetSolverTest01();
        FollowSetSolverTest01();
        FollowSetSolverTest02();
        FollowSetSolverTest03();
    }

};


#endif //SIMPLE_COMPILER_LLTEST_H
