/*
Author: xinhaitianze
Name : Nyasm
Date : 2025 / 8 / 2 21：43

Nyasm = Nya + asm
顺便一提，nya这一发音，在我老家方言中，有“什么”的意思，
不过原本应读“ni nya”只是说的太快，简略了些
*/


#include <iostream>
#include <locale>
#include "./include/open.h"
#include "./include/token.h"
#include "./include/error.h"
#include "./include/processing.h"
#include "./include/lexer.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char* argv[])
{
    // 没这玩意嫌人的（讨厌的）windows控制台就死活不正常显示中文
    #ifdef _WIN32
        SetConsoleOutputCP(65001);
        SetConsoleCP(65001);
    #endif
    float ver = 0.01;
    if (argc < 2) {
        std::cout << "nya? Do you mean you don't know how to run a Nyasm file?\n"
                  <<"try this: \033[92;40mnyasm\033[0m \033[93;40m-h\033[0m\n";
        exit(0);
    }
    else if (std::string(argv[1]) == "--version" || std::string(argv[1]) == "-v") {
        std::cout << "\033[1;36mNyasm \033[0mVer " << ver << " (Test Version)\n";
        exit(0);
    }
    else if (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h") {
        std::cout << "\033[1;36mNyasm \033[0mVer " << ver << "\n"
                  << "\033[92;40mUsage\033[0m: nyasm <source_file>\n\n"
                  << "Options:\n"
                  << "  \033[93;40m-v\033[0m, \033[93;40m--version\033[0m       Show version information\n"
                  << "  \033[93;40m-h\033[0m, \033[93;40m--help\033[0m          Show this help message\n\n"
                  << "Made by \033[94;40mxinhaitianze\033[0m";
        exit(0);
    }
    std::cout << "\033[94;40mReading File...\033[37;40m\n";
    std::string code = read(argv[1]);
    if (code == "!Error") {
        std::cerr << "\033[91;40mReadError\033[37;40m: Unable to read the file: \033[93;40m" << argv[1] << "\033[37;40m\n";
        std::cerr << "\033[96;40mNya! Don't worry! Check your file name\033[37;40m!\n";
        exit(1);
    }

    std::string copyCode = code;
    // 通用语法检查
    checkSyntax(copyCode, std::string(argv[1]));
    // 语法检查/语义检查

    //std::cout << code;
    //std::cout << "\033[94;40mConverting code...\033[37;40m\n";
    
	std::string finalCode = Uprocessing(copyCode);
    std::cout << "处理后代码：\n" << finalCode << std::endl;


    auto tokens = tokenize_code(finalCode);
    std::cout << "Tokens:\n";
    for (const auto& token : tokens) {
        std::cout << "[" << token << "] ";
    }
    std::cout << "\n";

    
    

    // test
    lex(tokens);
    finalCode.clear();
    copyCode.clear();
    std::cout << "Congratulations, your code has no syntax errors.\n";
    return 0;
}
