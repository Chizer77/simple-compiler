#include "include/util/FileIO.h"
#include "include/util/Init.h"
#include "iostream"
#include "cstring"
#include "frontend/Lexer.h"

//输入文件input_file, 目标文件target_file
std::string input_file, target_file;

/***
 * 首先进行编译：
 * 1. 在simple-compiler文件夹下新建build目录
 * 2. 在build目录下cmd运行cmake ..
 * 3. 运行cmake --build .
 *
 * 使用Terminal命令分析文件 compiler -S -o <target_file> <input_file>
 * 或 start compiler -S -o <target_file> <input_file>
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
    //以下五个参数会造成内存泄漏，仅进行DEBUG
    argv[0] = ".\\compiler";
    argv[1] = "-S";
    argv[2] = "-o";
    //由于idea将编译后文件放在cmake-build-debug中，因此需要出该文件夹
    argv[3] = "../output/out.txt";
    argv[4] = "../test/example.txt";
    parse_args(argc, argv);
    std::cout << "hello world" << std::endl;
    char* fileStr = FileIO::read(input_file.c_str());

    Lexer::init();
    auto *lexer = new Lexer();
    lexer->lex(fileStr);
    std::cout << lexer->tokenList;
    FileIO::write(target_file.c_str(), lexer->tokenList.toString());
    FileIO::close();

    lexer->clear();
    Lexer::destroy();
    free(fileStr);
    delete lexer;
}