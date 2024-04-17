#include "iostream"
#include <io.h>
#include <regex>
#include "cassert"
#include "util/FileIO.h"

char* FileIO::read(const char *filename) {
    input = fopen(filename, "r");
    assert(input != nullptr);
    //读入文件开头至结尾的字符
    fseek(input, SEEK_SET, SEEK_END);
    unsigned int fileSize = ftell(input) + 1;
    //将input指针置于文件开头
    rewind(input);
    // () 初始化内存
    char *fileStr = new char[fileSize + 1]();
    //读入内存
    fread(fileStr, 1, fileSize, input);
//    fileStr[fileSize] = 0;
    return fileStr;
}

void FileIO::write(const char *filename, const std::string& s) {
    //拆分文件路径
    std::match_results<std::string::const_iterator> names;
    std::regex Pattern("(^.+?\/)(.+).", std::regex_constants::extended);
    std::string fileString = std::string(filename);
    bool state = std::regex_search(fileString, names, Pattern);
    if(state) {
        //创建目录
        mkdir(names[1].str().c_str());
        output = fopen(filename, "w");
        fprintf(output, "%s", s.c_str());
    }else {
        std::cerr << "Invalid directory." << std::endl;
        exit(1);
    }
}

void FileIO::close() {
    fclose(input);
    fclose(output);
}