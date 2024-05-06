#include "frontend/Token.h"
#include <iostream>

std::ostream &operator<<(std::ostream &ostream, const Token &token) {
    ostream << "<" << Token::list[token.token_type] << " " << token.content << ">\n";
    return ostream;
}

std::ostream &operator<<(std::ostream &ostream, const TokenList &tokenList) {
    for(Token *token: tokenList.tokens) {
        ostream << *token;
    }
    return ostream;
}