#include "frontend/Lexer.h"
#include "util/LexerUtil.h"
#include "iostream"

std::unordered_set<std::string> Lexer::keyWordSet;

std::unordered_set<Lexer::lexDfa*, Lexer::lexDfa::lexDfaHasher> Lexer::lexDFASet;

void Lexer::init() {
    for(Token::TokenType t = Token::VOID; t <= Token::WHILE; t = (Token::TokenType)(t + 1)) {
        keyWordSet.insert(Token::list[t]);
    }
    //常量
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
    lexDFASet.insert(new lexDfa(Token::IDENT, numDfa));

    //
}

//对fileStr内容进行分析
void Lexer::lex(const std::string& fileStr) {
    auto *token = new Token(Token::VOID, "void");
    tokenList.append(token);
    for(const auto& it: lexDFASet) {
        std::cout << it->dfa->s.size();
    }
    std::cout<<fileStr;
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