#include <algorithm>
#include "string"
#include "util/ParseUtil.h"
#include "frontend/Nfa.h"
#include "unordered_map"
#include "stack"


const std::unordered_map<char, int> ParseUtil::priority = {
        {Nfa::KLEENE_STATE, 0},
        {Nfa::CONNECTION_STATE, 1},
        {Nfa::UNION_STATE, 2},
        {Nfa::LPARENT_STATE, 3}
};

/**
 * 将正规式转换为后缀正规式，例：a|(a|b)^cd ---> aab|^cd&&|
 * @param exp 初始正规式
 * @return 转换为后缀正规式
 */
std::string ParseUtil::toSuffix(const std::string& exp) {
    return toSuffixR(format(exp));
}

bool isVaild(char c) {
//    return c > 31 && c < 127;
    return (c > 45 && c < 58) || (c > 96 && c < 123);
}

std::string ParseUtil::format(const std::string& exp) {
    std::string str = exp;
    for (size_t i = 1; i < str.length(); i++) {
        char c = str[i];
        char cPre = str[i-1];
        if (c == Nfa::LPARENT_STATE) {
            if (isVaild(cPre) || cPre == Nfa::KLEENE_STATE || cPre == Nfa::RPARENT_STATE) {
                str.insert(i, 1, Nfa::CONNECTION_STATE);
                i++;
            }
        } else if (isVaild(c)) {
            if (isVaild(cPre) || cPre == Nfa::RPARENT_STATE || cPre == Nfa::KLEENE_STATE) {
                str.insert(i, 1, Nfa::CONNECTION_STATE);
                i++;
            }
        }
    }
    return str;
}

std::string ParseUtil::toSuffixR(const std::string& exp) {
    std::stack<char> stack;
    std::string res;
    for (char c : exp) {
        if (isVaild(c)) {
            res += c;
        } else if (c == Nfa::LPARENT_STATE) {
            stack.push(c);
        } else if (c == Nfa::RPARENT_STATE) {
            while (stack.top() != Nfa::LPARENT_STATE) {
                res += stack.top();
                stack.pop();
            }
            stack.pop();
        } else {
            while (!stack.empty() && priority.at(c) > priority.at(stack.top())) {
                res += stack.top();
                stack.pop();
            }
            stack.push(c);
        }
    }
    while (!stack.empty()) {
        res += stack.top();
        stack.pop();
    }
    return res;
}