#ifndef SIMPLE_COMPILER_PARSER_H
#define SIMPLE_COMPILER_PARSER_H


class Parser {
public:
    Parser() = default;

    /**
     * 通过读入数据初始化语法分析器Parser
     */
    static void init();

    /**
     * 语法分析
     */
    void parse(const std::string &s);
};


#endif //SIMPLE_COMPILER_PARSER_H
