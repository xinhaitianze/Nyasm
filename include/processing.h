#pragma once
#include <vector>
#include <string>  
#include "./open.h"
#include "./error.h"
#include <iostream>


// 处理代码并打印行
std::string Uprocessing(const std::string& code) {
    std::vector<std::string> lines = split_lines(code);
	std::string finalCode; // 用于存储最终代码

    for (size_t i = 0; i < lines.size(); ++i) {
        
        // 此处处理using语句
        if (lines[i].substr(0, 5) == "using") {
			std::string usingName = "./using/" + trim(lines[i].substr(6)) + ".nya"; // 获得using的文件的路径
			std::string usingCode = read(usingName.c_str());                        // 读取using的内容
            if (usingCode == "!Error") {
                std::cout << "\nline " << i+1 << ":\n    " << lines[i] << "\n";
                std::cout << "\033[91;40mReadError\033[37;40m: Unable to read the Using file: \033[93;40m" << trim(lines[i].substr(6)) << "\033[37;40m\n";
                exit(1);
            }
            checkSyntax(usingCode, usingName.c_str());                                               // 检查using的语法
			finalCode += usingCode + "\n";                                          // 将using的内容添加到最终代码中
		}
		else finalCode += lines[i] + "\n";                                          // 将当前行添加到最终代码中
    }

	//std::cout << finalCode << std::endl;
    return finalCode;
}

/*
此函数提供代码函数的解析与处理（函数名、参数、代码体等）
返回：一个包含函数的 vector
*/
struct Function {
    std::string name;
    std::string params;
    std::string body;
};

std::vector<Function> lex(const std::vector<std::string>& tokens) {
    std::vector<Function> functions;
    Function currentFunc;
    bool isFn = false, isName = false, isParams = false, isBody = false;
    int braceLevel = 0;
    std::stringstream paramsBuffer, bodyBuffer;

    for (size_t i = 0; i < tokens.size(); ++i) {
        const auto& token = tokens[i];

        if (token == "fn") {
            isFn = true;
            currentFunc = Function();  // 重置当前函数
        } 
        // 提取函数名
        else if (isFn && currentFunc.name.empty()) {
            currentFunc.name = token;
            isFn = false;
            isParams = true;
            paramsBuffer.str("");  // 清空参数缓冲区
        } 
        // 提取参数列表（直到遇到 "{"）
        else if (isParams) {
            if (token == "{") {
                currentFunc.params = paramsBuffer.str();
                isParams = false;
                isBody = true;
                braceLevel = 1;
                bodyBuffer.str("");  // 清空函数体缓冲区
            } else {
                paramsBuffer << token << " ";
            }
        } 
        // 提取函数体（处理嵌套花括号）
        else if (isBody) {
            if (token == "{") {
                braceLevel++;
                bodyBuffer << "{ ";
            } else if (token == "}") {
                braceLevel--;
                if (braceLevel == 0) {
                    currentFunc.body = bodyBuffer.str();
                    functions.push_back(currentFunc);
                    isBody = false;
                } else {
                    bodyBuffer << "} ";
                }
            } else {
                // 标准化符号间隔（修复 "1 . 0" 问题）
                if (!bodyBuffer.str().empty() && 
                    !ispunct(bodyBuffer.str().back()) && 
                    !ispunct(token[0])) {
                    bodyBuffer << " ";
                }
                bodyBuffer << token;
            }
        }
    }

    // 打印结果
    std::cout << "=======================Function======================\n";
    for (const auto& func : functions) {
        std::cout << "Function: " << func.name << func.params << " {\n";
        std::cout << "    " << func.body << "\n}\n";
    }

    return functions;
}