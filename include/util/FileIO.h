#ifndef SIMPLE_COMPILER_FILEIO_H
#define SIMPLE_COMPILER_FILEIO_H


#include <string>

class FileIO {
public:
    /***
     * 输入流input，输出流output
     */
    static FILE* input;
    static FILE* output;

    /**
     * 读取文件路径filename内字符，存入fileStr中，文件路径需存在
     * @param filename 文件路径
     */
    static char* read(const char *filename);

    /**
     * 将s写入文件filename
     * @param filename 输出文件路径
     * @param s 输出内容
     */
    static void write(const char *filename, const std::string& s);

    /**
     * 关闭各文件流
     */
    static void close();
};


#endif //SIMPLE_COMPILER_FILEIO_H
