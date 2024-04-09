#include <algorithm>
#include "string"
#include "util/ParseUtil.h"
#include "unordered_map"
#include "stack"


const std::unordered_map<char, int> ParseUtil::priority = {
        {'^', 0},
        {'&', 1},
        {'|', 2},
        {'(', 3}
};

/**
 * 将正规式转换为后缀正规式，例：a|(a|b)^cd ---> aab|^cd**|
 * @param exp 初始正规式
 * @return 转换为后缀正规式
 */
std::string ParseUtil::toSuffix(const std::string& exp) {
    return toSuffixR(format(exp));
}

std::string ParseUtil::format(const std::string& exp) {
    std::string str = exp;
    str.erase(std::remove_if(str.begin(), str.end(), [](char c) { return c == ' '; }), str.end());
    for (size_t i = 1; i < str.length(); i++) {
        char c = str[i];
        char cPre = str[i-1];
        if (c == '(') {
            if (std::islower(cPre) || cPre == '^') {
                str.insert(i, "&");
                i++;
            }
        } else if (std::islower(c)) {
            if (std::islower(cPre) || cPre == ')' || cPre == '^') {
                str.insert(i, "&");
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
        if (std::islower(c)) {
            res += c;
        } else if (c == '(') {
            stack.push(c);
        } else if (c == ')') {
            while (stack.top() != '(') {
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