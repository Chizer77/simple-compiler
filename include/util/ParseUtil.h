#ifndef SIMPLE_COMPILER_PARSEUTIL_H
#define SIMPLE_COMPILER_PARSEUTIL_H

#include <unordered_map>
#include "string"

/**
 * 中缀转后缀
 */
class ParseUtil {
private:
    static const std::unordered_map<char, int> priority;
    static std::string format(const std::string& exp);
    static std::string toSuffixR(const std::string& exp);
public:
    static std::string toSuffix(std::string exp);
};

#endif //SIMPLE_COMPILER_PARSEUTIL_H
