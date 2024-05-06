#include "frontend/Lexer.h"
#include "util/LexerUtil.h"
#include "iostream"

std::unordered_map<std::string, Token::TokenType> Lexer::keyWordSet;

std::unordered_set<Lexer::lexDfa*, Lexer::lexDfa::lexDfaHasher> Lexer::lexDFASet;

void Lexer::init() {
    // 初始化关键字集合keyWordSet
    for(Token::TokenType t = Token::VOID; t <= Token::WHILE; t = (Token::TokenType)(t + 1)) {
        keyWordSet[Token::list[t]] = t;
    }

    // 常量
    std::string num = "0";    //含0
    for(int i = 1; i < 10; i++) {
        num = LexerUtil::lexUnion(num, std::to_string(i));
    }
    std::string num_ep0 = std::to_string(1);    //不含0
    for(int i = 2; i < 10; i++) {
        num_ep0 = LexerUtil::lexUnion(num_ep0, std::to_string(i));
    }
    num = LexerUtil::lexBrace(num);
    num_ep0 = LexerUtil::lexBrace(num_ep0);
    num = LexerUtil::lexKleene(num);
    std::string integer = LexerUtil::lexConnection(num_ep0, num);
    integer = LexerUtil::lexUnion("0", integer);
    std::string decimal = LexerUtil::lexConnection(integer, ".");
    decimal = LexerUtil::lexConnection(decimal, num);
    decimal = LexerUtil::lexConnection(decimal, num_ep0);
    std::string lexNumber = LexerUtil::lexUnion(integer, decimal);
    Dfa *numDfa = Dfa::Generation(lexNumber);
    lexDFASet.insert(new lexDfa(Token::NUMBER, numDfa));

    // 变量标识符（未限定长度不超过32个字符）
    std::string lowercase = "a";
    for(int i = 'b'; i <= 'z'; i++) {
        char c = static_cast<char>(i);
        std::string tmp(1, c);
        lowercase = LexerUtil::lexUnion(lowercase, tmp);
    }
    std::string uppercase = "A";
    for(int i = 'B'; i <= 'Z'; i++) {
        char c = static_cast<char>(i);
        std::string tmp(1, c);
        uppercase = LexerUtil::lexUnion(uppercase, tmp);
    }
    std::string initial = LexerUtil::lexUnion(lowercase, uppercase);
    initial = LexerUtil::lexUnion(initial, "_");
    initial = LexerUtil::lexBrace(initial);
    std::string other = LexerUtil::lexUnion(lowercase, uppercase);
    other = LexerUtil::lexUnion(other, "_");
    for(int i = 0; i <= 9; i++) {
        other = LexerUtil::lexUnion(other, std::to_string(i));
    }
    other = LexerUtil::lexBrace(other);
    other = LexerUtil::lexKleene(other);
    std::string lexVariable = initial;
    lexVariable = LexerUtil::lexConnection(lexVariable, other);
    Dfa *letterDfa = Dfa::Generation(lexVariable);
    lexDFASet.insert(new lexDfa(Token::IDENT, letterDfa));

    // +-*/%=(){}!
    Dfa *addDfa = Dfa::Generation("+");
    lexDFASet.insert(new lexDfa(Token::ADD, addDfa));
    Dfa *subDfa = Dfa::Generation("-");
    lexDFASet.insert(new lexDfa(Token::SUB, subDfa));
    Dfa *mulDfa = Dfa::Generation("*");
    lexDFASet.insert(new lexDfa(Token::MUL, mulDfa));
    Dfa *divDfa = Dfa::Generation("/");
    lexDFASet.insert(new lexDfa(Token::DIV, divDfa));
    Dfa *modDfa = Dfa::Generation("%");
    lexDFASet.insert(new lexDfa(Token::MOD, modDfa));
    Dfa *assignDfa = Dfa::Generation("=");
    lexDFASet.insert(new lexDfa(Token::ASSIGN, assignDfa));
    Dfa *lparentDfa = Dfa::Generation("(");
    lexDFASet.insert(new lexDfa(Token::LPARENT, lparentDfa));
    Dfa *rparentDfa = Dfa::Generation(")");
    lexDFASet.insert(new lexDfa(Token::RPARENT, rparentDfa));
    Dfa *lbraceDfa = Dfa::Generation("{");
    lexDFASet.insert(new lexDfa(Token::LBRACE, lbraceDfa));
    Dfa *rbraceDfa = Dfa::Generation("}");
    lexDFASet.insert(new lexDfa(Token::RBRACE, rbraceDfa));
    Dfa *notDfa = Dfa::Generation("!");
    lexDFASet.insert(new lexDfa(Token::NOT, notDfa));

    // 条件运算符Operator
    // 单目
    Dfa * ltDfa = Dfa::Generation("<");
    lexDFASet.insert(new lexDfa(Token::LT, ltDfa));
    Dfa * gtDfa = Dfa::Generation(">");
    lexDFASet.insert(new lexDfa(Token::GT, gtDfa));
    // 双目
    Dfa * landDfa = Dfa::Generation("&&");
    lexDFASet.insert(new lexDfa(Token::LAND, landDfa));
    Dfa * lorDfa = Dfa::Generation("||");
    lexDFASet.insert(new lexDfa(Token::LOR, lorDfa));
    Dfa * leDfa = Dfa::Generation("<=");
    lexDFASet.insert(new lexDfa(Token::LE, leDfa));
    Dfa * geDfa = Dfa::Generation(">=");
    lexDFASet.insert(new lexDfa(Token::GE, geDfa));
    Dfa * eqDfa = Dfa::Generation("==");
    lexDFASet.insert(new lexDfa(Token::EQ, eqDfa));
    Dfa * neDfa = Dfa::Generation("!=");
    lexDFASet.insert(new lexDfa(Token::NE, neDfa));

    // ,
    Dfa *commaDfa = Dfa::Generation(",");
    lexDFASet.insert(new lexDfa(Token::COMMA, commaDfa));
    // ;
    Dfa *semiDfa = Dfa::Generation(";");
    lexDFASet.insert(new lexDfa(Token::SEMI, semiDfa));

    //  \t \n
    std::string split = " ";
    split = LexerUtil::lexUnion(split, "\t");
    split = LexerUtil::lexUnion(split, "\n");
    Dfa *splitDfa = Dfa::Generation(split);
    lexDFASet.insert(new lexDfa(Token::SPLIT, splitDfa));
}

//对fileStr内容进行分析
void Lexer::lex(const std::string& fileStr) {
    int idx = 0;
    int size = (int) fileStr.size();
    while(idx < size) {
        // 最长匹配原则
        std::string maxToken;
        Token::TokenType maxTokenType;
        int maxLen = 0;
        for(auto lexDfa: lexDFASet) {
            Dfa *dfa = lexDfa->dfa;
            Token::TokenType type = lexDfa->type;
            //当前字符位置pos, 当前记号token
            int pos = idx;
            std::string token;
            //当前结点位置now
            int now = dfa->s0;
            bool isContinue = true;
            while(isContinue) {
                isContinue = false;
                if(pos >= size) break;
                for(Edge e: dfa->edges) {
                    if(e.start == now && e.alpha == fileStr[pos]) {
                        now = e.target;
                        token.push_back(e.alpha);
                        pos++;
                        isContinue = true;
                        break;
                    }
                }
            }
            if(dfa->target.find(now) != dfa->target.end()) {
                if(maxLen < token.size()) {
                    maxLen = (int)token.size();
                    maxToken = token;
                    maxTokenType = type;
                }
            }
        }

        auto ident = keyWordSet.find(maxToken);
        if(ident != keyWordSet.end()) {
            tokenList.append(new Token(ident->second, maxToken));
            idx += maxLen;
        }else if(maxTokenType == Token::IDENT && maxLen > 32) { // 限定变量长度不超过32个字符
            idx += 32;
        }else if(maxTokenType == Token::SPLIT) {
            idx += maxLen;
        }else {
            tokenList.append(new Token(maxTokenType, maxToken));
            idx += maxLen;
        }
    }
}

void Lexer::destroy() {
    for(auto it: lexDFASet) {
        free(it->dfa);
        free(it);
    }
    lexDFASet.clear();
}

void Lexer::clear() {
    tokenList.clear();
}