#include "iostream"
#include "include/util/Init.h"
#include <cstring>

//输入文件input_file, 目标文件target_file
std::string input_file, target_file;

/***
 * 首先进行编译：g++ compiler.cpp -o "compiler"
 *
 * 使用Terminal命令分析文件 compiler -S -o <target_file> <input_file>
 * 例：.\compiler -S -o "out.txt" "test/example.txt"
 * @param argc
 * @param argv
 */
void parse_args(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (i + 2 < argc && strcmp(argv[i], "-S") == 0 && strcmp(argv[i + 1], "-o") == 0) {
            target_file.assign(argv[i + 2]);
            i += 2;
            continue;
        }
        input_file.assign(argv[i]);
    }
    if (input_file.empty()) {
        //输出到标准错误的ostream对象
        std::cerr << "error: can not find a input file." << std::endl;
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    argc = 5;
    argv[0] = ".\\compiler";
    argv[1] = "-S";
    argv[2] = "-o";
    //由于idea将编译后文件放在cmke-buile-debug中，因此需要出该文件夹
    argv[3] = "../output/out.txt";
    argv[4] = "../test/example.txt";
    parse_args(argc, argv);
    std::cout << "hello world" << std::endl;
    char* fileStr = FileIO::read(input_file.c_str());
    FileIO::write(target_file.c_str(), fileStr);
    FileIO::close();
}