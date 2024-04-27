#ifndef SIMPLE_COMPILER_CFG_H
#define SIMPLE_COMPILER_CFG_H

#include "unordered_set"
#include "vector"

/**
 * 产生式 A -> α
 * 开始符号 start (A)
 * 语法 grammar (α)
 */
class Productions {
public:
    int start;
    std::vector<int> grammar;

    Productions(int s, std::vector<int>& g) {
        this->start = s;
        g.swap(this->grammar);
    }
    ~Productions() = default;

    bool operator == (const Productions & p) const {
        if (start != p.start) return false;
        for(int i = 0; i < p.grammar.size(); i++) {
            if(p.grammar[i] != grammar[i]) return false;
        }
        return true;
    }
    struct ProductionsHasher final {
        unsigned long long operator()(const Productions& p) const{
            unsigned long long hash = std::hash<int>()(p.start);
            for(int g: p.grammar) {
                hash ^= std::hash<int>()(g);
            }
            return hash;
        }
    };
};

/**
 * 开始符号 start
 * 非终结符集 nonter
 * 终结符集 ter
 * 产生式集合 products
 */
class CFG {
private:
    //符号标识
    static int SYMBOL_ID;
public:
    int start;
    std::unordered_set<int> nonter;
    std::unordered_set<int> ter;
    std::unordered_set<Productions, Productions::ProductionsHasher> products;

    static int newId() {
        return SYMBOL_ID++;
    }
};
#endif //SIMPLE_COMPILER_CFG_H
