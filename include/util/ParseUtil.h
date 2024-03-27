#ifndef SIMPLE_COMPILER_PARSEUTIL_H
#define SIMPLE_COMPILER_PARSEUTIL_H

#include "string"

/**
 * 中缀转后缀
 */
class ParseUtil {
public:
    static std::string toSuffix(std::string exp);
};

#endif //SIMPLE_COMPILER_PARSEUTIL_H
