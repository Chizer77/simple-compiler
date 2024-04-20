#include "frontend/Token.h"
#include <iostream>

std::ostream &operator<<(std::ostream &ostream, const Token &token) {
    ostream << "<" << Token::list[token.token_type] << " " << token.content << ">";
    return ostream;
}
