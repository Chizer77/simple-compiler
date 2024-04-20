#ifndef SIMPLE_COMPILER_LEXERUTIL_H
#define SIMPLE_COMPILER_LEXERUTIL_H

#include "string"
#include "../frontend/Nfa.h"

/**
 * 用于书写正规式
 */
class LexerUtil {
public:

    /**
     * 并操作
     * @param s1
     * @param s2
     * @return s1 并 s2
     */
    static std::string lexUnion(const std::string& s1, const std::string& s2) {
        std::string s = s1;
        s += Nfa::UNION_STATE;
        s += s2;
        return s;
    }

    /**
     * 与操作
     * @param s1
     * @param s2
     * @return s1 与 s2
     */
    static std::string lexConnection(const std::string& s1, const std::string& s2) {
        std::string s = s1;
        s += Nfa::CONNECTION_STATE;
        s += s2;
        return s;
    }

    /**
     * 闭包操作
     * @param s1
     * @return s1 闭包符号
     */
    static std::string lexKleene(const std::string& s1) {
        std::string s = s1;
        s += Nfa::KLEENE_STATE;
        return s;
    }

    /**
     * 添加括号
     * @param s1
     * @return ( s1 )
     */
    static std::string  lexBrace(const std::string& s1) {
        std::string s;
        s += Nfa::LPARENT_STATE;
        s += s1;
        s += Nfa::RPARENT_STATE;
        return s;
    }
};


#endif //SIMPLE_COMPILER_LEXERUTIL_H
