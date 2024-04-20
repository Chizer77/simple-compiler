#ifndef SIMPLE_COMPILER_TOKEN_H
#define SIMPLE_COMPILER_TOKEN_H


#include <vector>
#include "string"

class Token {
public:
    enum TokenType {
        // 变量标识符
        IDENT = 0,
        //数字
        NUMBER,

        //关键字Keyword
        //基本数据类型
        VOID,
        INT,
        FLOAT,
        DOUBLE,
        CHAR,

        //流程控制
        BREAK,
        CONTINUE,
        RETURN,

        //分支控制
        IF,
        ELSE,
        SWITCH,
        CASE,
        DEFAULT,

        //循环结构
        FOR,
        DO,
        WHILE,

        // 条件运算符Operator
        // 双目
        LAND,
        LOR,
        LE,
        GE,
        EQ,
        NE,
        // 单目
        LT,
        GT,
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        NOT,    // !
        ASSIGN, // =
        SEMI,   // ;
        COMMA,  // ,
        LPARENT,    // (
        RPARENT,    // )
        LBRACK, // [
        RBRACK, // ]
        LBRACE, // {
        RBRACE, // }

        //标准字符
        STR_CON, // ""
    };

    /**
     * 记号类型 token_type
     * 具体字符串 content
     */
    TokenType token_type;
    std::string content;

    /**
     * 记号对应类型(字符串形式)
     */
    static std::string list[45];

    Token(TokenType type, std::string cont) : token_type(type), content(std::move(cont)) {}
    ~Token() = default;

    /**
     * 输出记号流
     * @param ostream
     * @param token
     * @return
     */
    friend std::ostream &operator<<(std::ostream &ostream, const Token &token);
};

class TokenList {
public:
    // 记号集合tokens
    std::vector<Token *> tokens;

    void append(Token *token) {
        tokens.push_back(token);
    }

    Token* front() {
        return tokens.front();
    }

    Token* back() {
        return tokens.back();
    }

    void pop_back() {
        tokens.pop_back();
    }

    bool empty() {
        return tokens.empty();
    }

    void clear() {
        while (!tokens.empty()) {
            Token *token = tokens.back();
            free(token);
            tokens.pop_back();
        }
        tokens.clear();
    }

    TokenList() = default;

    ~TokenList() {
        while (!tokens.empty()) {
            Token *token = tokens.back();
            free(token);
            tokens.pop_back();
        }
        tokens.clear();
    }
};

#endif //SIMPLE_COMPILER_TOKEN_H
