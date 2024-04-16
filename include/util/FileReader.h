#ifndef SIMPLE_COMPILER_FILEREADER_H
#define SIMPLE_COMPILER_FILEREADER_H


#include <iostream>
#include <cassert>

class FileReader {
public:
    static FILE* input;
    static FILE* output;
    static char* fileStr;

    static void reader(const char *filename) {
        input = fopen(filename, "r");
        assert(input != nullptr);
        //读入文件开头至结尾的字符
        fseek(input, SEEK_SET, SEEK_END);
        unsigned int fileSize = ftell(input) + 1;
        //将input指针置于文件开头
        rewind(input);
        fileStr = new char[fileSize + 1](); // use () to initialize the memory
        //读入内存
        fread(fileStr, 1, fileSize, input);
        fileStr[fileSize] = 0;
    }
};


#endif //SIMPLE_COMPILER_FILEREADER_H
