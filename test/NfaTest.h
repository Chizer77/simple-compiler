#ifndef SIMPLE_COMPILER_NFATEST_H
#define SIMPLE_COMPILER_NFATEST_H


class NfaTest {
public:

    /**
     * 创建新边测试
     */
    static void nfaInitTest01();

    /**
     * 闭包^操作测试
     */
    static void kleeneTest01();

    /**
     * 连接&操作测试
     */
     static void concatenationTest01();
    /**
     * 或|操作测试
     */
    static void unionTest01();

    /**
     * 前缀转后缀测试
     */
    static void parseUtilTest();

    static void run() {
        nfaInitTest01();
        kleeneTest01();
        concatenationTest01();
        unionTest01();
        parseUtilTest();
    };
};


#endif //SIMPLE_COMPILER_NFATEST_H
