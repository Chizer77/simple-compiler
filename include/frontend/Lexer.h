#ifndef SIMPLE_COMPILER_LEXER_H
#define SIMPLE_COMPILER_LEXER_H


#include <string>
#include <unordered_set>
#include "Token.h"
#include "Dfa.h"

/**
 * 词法分析器 Lexer
 */
class Lexer {
private:
    struct lexDfa {
        Token::TokenType type = Token::STR_CON;
        Dfa* dfa{};

        bool operator ==(const lexDfa& ld) const {
            return type == ld.type;
        }

        struct lexDfaHasher final {
            unsigned long long operator()(const lexDfa* ld) const{
                return std::hash<int>()(ld->type);
            }
        };

        lexDfa() = default;
        lexDfa(Token::TokenType t, Dfa* d) {
            type = t;
            dfa = d;
        }
    };

    /**
     * 关键字(字符串)集合 keyWordSet
     */
    static std::unordered_set<std::string> keyWordSet;

    /**
     * 语法DFA集合
     */
    static std::unordered_set<lexDfa*, lexDfa::lexDfaHasher> lexDFASet;

public:

    Lexer() = default;

    /**
     * TODO:初始化keyWordSet和lexDFASet
     */
    static void init();

    /**
     * 清空keyWordSet和lexDFASet
     */
    static void destroy();

    /**
     * 转换后记号表 tokenList
     */
    TokenList tokenList;

    /**
     * 情空记号表
     */
    void clear();

    /**
     * TODO:对fileStr内容进行分析，生成记号流并存入tokenList
     * @param fileStr 待分析程序代码
     */
    void lex(const std::string& fileStr);

};


#endif //SIMPLE_COMPILER_LEXER_H
