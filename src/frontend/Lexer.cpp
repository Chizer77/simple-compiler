#include "frontend/Lexer.h"
#include "util/LexerUtil.h"
#include "util/FileIO.h"

std::unordered_map<std::string, Token::TokenType> Lexer::keyWordSet;

std::unordered_set<Lexer::lexDfa *, Lexer::lexDfa::lexDfaHasher> Lexer::lexDFASet;


// 字符串转int
int getNum(const std::string &txt) {
    int ans = 0;
    for (char i: txt) {
        ans *= 10;
        ans += i - '0';
    }
    return ans;
}

void Lexer::init(const std::string &configFileName) {
    // 初始化关键字集合keyWordSet
    for (Token::TokenType t = Token::VOID; t <= Token::WHILE; t = (Token::TokenType) (t + 1)) {
        keyWordSet[Token::list[t]] = t;
    }
    // 从文件输入C++语言定义NFA
    FILE *input = fopen(configFileName.c_str(), "r");
    if (input != nullptr) {
        std::string lexDfaTxt = FileIO::read(configFileName.c_str());
        int idx = 0;
        int op = 0;
        Token::TokenType type;
        Dfa *dfa = new Dfa();
        while (idx < lexDfaTxt.size()) {
            std::string s;
            if (op == 0) {
                while (idx < lexDfaTxt.size() && lexDfaTxt[idx] != FileIO::fileNewLine) {
                    s += lexDfaTxt[idx++];
                }
                type = (Token::TokenType) getNum(s);
                op++;
                idx++;
            } else if (op == 1) {
                while (idx < lexDfaTxt.size() && lexDfaTxt[idx] != FileIO::fileNewLine) {
                    s += lexDfaTxt[idx++];
                }
                dfa->s0 = getNum(s);
                op++;
                idx++;
            } else if (op == 2) {
                while (idx < lexDfaTxt.size() && lexDfaTxt[idx] != FileIO::fileNewLine) {
                    s += lexDfaTxt[idx++];
                }
                std::string ts;
                int dfa_s;
                for (char i: s) {
                    if (i == FileIO::fileBlank) {
                        dfa_s = getNum(ts);
                        dfa->s.insert(dfa_s);
                        ts.clear();
                        continue;
                    }
                    ts += i;
                }
                op++;
                idx++;
            } else if (op == 3) {
                while (idx < lexDfaTxt.size() && lexDfaTxt[idx] != FileIO::fileNewLine) {
                    s += lexDfaTxt[idx++];
                }
                std::string ts;
                int dfa_tar;
                for (char i: s) {
                    if (i == FileIO::fileBlank) {
                        dfa_tar = getNum(ts);
                        dfa->target.insert(dfa_tar);
                        ts.clear();
                        continue;
                    }
                    ts += i;
                }
                op++;
                idx++;
            } else if (op == 4) {
                while (idx < lexDfaTxt.size() && lexDfaTxt[idx] != FileIO::fileNewLine) {
                    s += lexDfaTxt[idx++];
                }
                for (char i: s) {
                    if (i == FileIO::fileBlank) continue;
                    dfa->alpha.insert(i);
                }
                op++;
                idx++;
            } else if (op == 5) {
                while (idx < lexDfaTxt.size() && lexDfaTxt[idx] != FileIO::fileNewLine) {
                    s += lexDfaTxt[idx++];
                }
                int edgeSize = getNum(s);
                s.clear();
                for (int i = 0; i < edgeSize; i++) {
                    idx++;
                    while (idx < lexDfaTxt.size() && lexDfaTxt[idx] != FileIO::fileNewLine) {
                        s += lexDfaTxt[idx++];
                    }
                    std::string ts;
                    int tim = 0;
                    int start, target;
                    char alpha;
                    for (char j: s) {
                        if (j == FileIO::fileBlank) {
                            if (tim == 0) {
                                start = getNum(ts);
                                ts.clear();
                                tim++;
                            } else if (tim == 1) {
                                alpha = ts[0];
                                ts.clear();
                                tim++;
                            }
                            continue;
                        }
                        ts += j;
                    }
                    target = getNum(ts);
                    dfa->edges.insert(Edge(start, target, alpha));
                    s.clear();
                }
                lexDFASet.insert(new lexDfa(type, dfa));
                dfa = new Dfa();
                op = 0;
                idx++;
            }
        }
        return;
    }
    // 常量
    std::string num = "0";    //含0
    for (int i = 1; i < 10; i++) {
        num = LexerUtil::lexUnion(num, std::to_string(i));
    }
    std::string num_ep0 = std::to_string(1);    //不含0
    for (int i = 2; i < 10; i++) {
        num_ep0 = LexerUtil::lexUnion(num_ep0, std::to_string(i));
    }
    num = LexerUtil::lexBrace(num);
    num_ep0 = LexerUtil::lexBrace(num_ep0);
    num = LexerUtil::lexKleene(num);
    std::string integer = LexerUtil::lexConnection(num_ep0, num);
    integer = LexerUtil::lexUnion("0", integer);
    std::string decimal = LexerUtil::lexConnection(integer, ".");
    decimal = LexerUtil::lexConnection(decimal, num);
    decimal = LexerUtil::lexConnection(decimal, num_ep0);
    std::string lexNumber = LexerUtil::lexUnion(integer, decimal);
    Dfa *numDfa = Dfa::Generation(lexNumber);
    lexDFASet.insert(new lexDfa(Token::NUMBER, numDfa));

    // 变量标识符（未限定长度不超过32个字符）
    std::string lowercase = "a";
    for (int i = 'b'; i <= 'z'; i++) {
        char c = static_cast<char>(i);
        std::string tmp(1, c);
        lowercase = LexerUtil::lexUnion(lowercase, tmp);
    }
    std::string uppercase = "A";
    for (int i = 'B'; i <= 'Z'; i++) {
        char c = static_cast<char>(i);
        std::string tmp(1, c);
        uppercase = LexerUtil::lexUnion(uppercase, tmp);
    }
    std::string initial = LexerUtil::lexUnion(lowercase, uppercase);
    initial = LexerUtil::lexUnion(initial, "_");
    initial = LexerUtil::lexBrace(initial);
    std::string other = LexerUtil::lexUnion(lowercase, uppercase);
    other = LexerUtil::lexUnion(other, "_");
    for (int i = 0; i <= 9; i++) {
        other = LexerUtil::lexUnion(other, std::to_string(i));
    }
    other = LexerUtil::lexBrace(other);
    other = LexerUtil::lexKleene(other);
    std::string lexVariable = initial;
    lexVariable = LexerUtil::lexConnection(lexVariable, other);
    Dfa *letterDfa = Dfa::Generation(lexVariable);
    lexDFASet.insert(new lexDfa(Token::IDENT, letterDfa));

    // +-*/%=(){}[]
    Dfa *addDfa = Dfa::Generation("+");
    lexDFASet.insert(new lexDfa(Token::ADD, addDfa));
    Dfa *subDfa = Dfa::Generation("-");
    lexDFASet.insert(new lexDfa(Token::SUB, subDfa));
    Dfa *mulDfa = Dfa::Generation("*");
    lexDFASet.insert(new lexDfa(Token::MUL, mulDfa));
    Dfa *divDfa = Dfa::Generation("/");
    lexDFASet.insert(new lexDfa(Token::DIV, divDfa));
    Dfa *modDfa = Dfa::Generation("%");
    lexDFASet.insert(new lexDfa(Token::MOD, modDfa));
    Dfa *assignDfa = Dfa::Generation("=");
    lexDFASet.insert(new lexDfa(Token::ASSIGN, assignDfa));
    Dfa *lparentDfa = Dfa::Generation("(");
    lexDFASet.insert(new lexDfa(Token::LPARENT, lparentDfa));
    Dfa *rparentDfa = Dfa::Generation(")");
    lexDFASet.insert(new lexDfa(Token::RPARENT, rparentDfa));
    Dfa *lbraceDfa = Dfa::Generation("{");
    lexDFASet.insert(new lexDfa(Token::LBRACE, lbraceDfa));
    Dfa *rbraceDfa = Dfa::Generation("}");
    lexDFASet.insert(new lexDfa(Token::RBRACE, rbraceDfa));
    Dfa *lbrackDfa = Dfa::Generation("[");
    lexDFASet.insert(new lexDfa(Token::LBRACK, lbrackDfa));
    Dfa *rbrackDfa = Dfa::Generation("]");
    lexDFASet.insert(new lexDfa(Token::RBRACK, rbrackDfa));

    // 条件运算符Operator
    // 单目
    Dfa *ltDfa = Dfa::Generation("<");
    lexDFASet.insert(new lexDfa(Token::LT, ltDfa));
    Dfa *gtDfa = Dfa::Generation(">");
    lexDFASet.insert(new lexDfa(Token::GT, gtDfa));
    Dfa *notDfa = Dfa::Generation("!");
    lexDFASet.insert(new lexDfa(Token::NOT, notDfa));
    // 双目
    Dfa *landDfa = Dfa::Generation("&&");
    lexDFASet.insert(new lexDfa(Token::LAND, landDfa));
    Dfa *lorDfa = Dfa::Generation("||");
    lexDFASet.insert(new lexDfa(Token::LOR, lorDfa));
    Dfa *leDfa = Dfa::Generation("<=");
    lexDFASet.insert(new lexDfa(Token::LE, leDfa));
    Dfa *geDfa = Dfa::Generation(">=");
    lexDFASet.insert(new lexDfa(Token::GE, geDfa));
    Dfa *eqDfa = Dfa::Generation("==");
    lexDFASet.insert(new lexDfa(Token::EQ, eqDfa));
    Dfa *neDfa = Dfa::Generation("!=");
    lexDFASet.insert(new lexDfa(Token::NE, neDfa));

    // ,
    Dfa *commaDfa = Dfa::Generation(",");
    lexDFASet.insert(new lexDfa(Token::COMMA, commaDfa));
    // ;
    Dfa *semiDfa = Dfa::Generation(";");
    lexDFASet.insert(new lexDfa(Token::SEMI, semiDfa));

    //  \t \n
    std::string split = " ";
    split = LexerUtil::lexUnion(split, "\t");
    split = LexerUtil::lexUnion(split, "\n");
    Dfa *splitDfa = Dfa::Generation(split);
    lexDFASet.insert(new lexDfa(Token::SPLIT, splitDfa));

    std::string dfaTxt;
    for (auto lexDfa: lexDFASet) {
        Token::TokenType token = lexDfa->type;
        Dfa *dfa = lexDfa->dfa;
        dfaTxt += std::to_string(token);
        dfaTxt += FileIO::fileNewLine;
        dfaTxt += std::to_string(dfa->s0);
        dfaTxt += FileIO::fileNewLine;
        for (auto s: dfa->s) {
            dfaTxt += std::to_string(s) + FileIO::fileBlank;
        }
        dfaTxt += FileIO::fileNewLine;
        for (auto tar: dfa->target) {
            dfaTxt += std::to_string(tar) + FileIO::fileBlank;
        }
        dfaTxt += FileIO::fileNewLine;
        for (auto alp: dfa->alpha) {
            dfaTxt += alp;
            dfaTxt += FileIO::fileBlank;
        }
        dfaTxt += FileIO::fileNewLine;
        dfaTxt += std::to_string(dfa->edges.size());
        dfaTxt += FileIO::fileNewLine;
        for (auto edge: dfa->edges) {
            dfaTxt += std::to_string(edge.start);
            dfaTxt += FileIO::fileBlank;
            dfaTxt += edge.alpha;
            dfaTxt += FileIO::fileBlank;
            dfaTxt += std::to_string(edge.target);
            dfaTxt += FileIO::fileNewLine;
        }
    }
    FileIO::write(configFileName.c_str(), dfaTxt);
}

//对fileStr内容进行分析
void Lexer::lex(const std::string &fileStr) {
    int idx = 0;
    int size = (int) fileStr.size();
    while (idx < size) {
        // 最长匹配原则
        std::string maxToken;
        Token::TokenType maxTokenType;
        int maxLen = 0;
        for (auto lexDfa: lexDFASet) {
            Dfa *dfa = lexDfa->dfa;
            Token::TokenType type = lexDfa->type;
            //当前字符位置pos, 当前记号token
            int pos = idx;
            std::string token;
            //当前结点位置now
            int now = dfa->s0;
            bool isContinue = true;
            while (isContinue) {
                isContinue = false;
                if (pos >= size) break;
                for (Edge e: dfa->edges) {
                    if (e.start == now && e.alpha == fileStr[pos]) {
                        now = e.target;
                        token.push_back(e.alpha);
                        pos++;
                        isContinue = true;
                        break;
                    }
                }
            }
            if (dfa->target.find(now) != dfa->target.end()) {
                if (maxLen < token.size()) {
                    maxLen = (int) token.size();
                    maxToken = token;
                    maxTokenType = type;
                }
            }
        }

        auto ident = keyWordSet.find(maxToken);
        if (ident != keyWordSet.end()) {
            tokenList.append(new Token(ident->second, maxToken));
            idx += maxLen;
        } else if (maxTokenType == Token::IDENT && maxLen > 32) { // 限定变量长度不超过32个字符
            tokenList.append(new Token(maxTokenType, maxToken.substr(0, 32)));
            idx += 32;
        } else if (maxTokenType == Token::SPLIT) {
            idx += maxLen;
        } else {
            tokenList.append(new Token(maxTokenType, maxToken));
            idx += maxLen;
        }
    }
}

void Lexer::destroy() {
    for (auto it: lexDFASet) {
        free(it->dfa);
        free(it);
    }
    lexDFASet.clear();
}

void Lexer::clear() {
    tokenList.clear();
}