#ifndef SIMPLE_COMPILER_NFATEST_H
#define SIMPLE_COMPILER_NFATEST_H


class nfaTest {
public:

    /**
     * 创建新边测试
     */
    static void nfaInitTest01();

    /**
     * 或|操作测试
     */
    static void unionTest01();

    static void parseUtilTest();

    static void run() {
        nfaInitTest01();
        unionTest01();
        parseUtilTest();
    };
};


#endif //SIMPLE_COMPILER_NFATEST_H
