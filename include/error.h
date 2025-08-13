#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "common.h"


using namespace std;

void error(std::string errorName, std::string errorDescription, int lineNumber, std::string lineCode, std::string errorLevel, std::string fileName) {
	std::cout << "\nIn " << fileName << " line " << lineNumber << ":\n";
	std::cout << "    " << lineCode << "\n";
	if (errorLevel == "error") {
		std::cout << "\033[91;40m";
	}
	else if (errorLevel == "warning") {
		std::cout << "\033[93;40m";
	}
	std::cout  << errorName << "\033[37;40m:" << errorDescription << "\n";
	if (errorLevel == "error") {
		exit(1);
	}
}

struct BracketInfo {
    int count = 0;
    size_t lastOpenLine = 0;      // 原startLine
    size_t lastOpenCol = 0;       // 原startCol
    string lastOpenLineContent;   // 原startLineContent
};

// 这恐怕是项目里最复杂的函数了for now,看以后能不能化简
void checkSyntax(const string& code, const string& fileName) {
    vector<string> lines = split_lines(code);
    
    // 括号跟踪（使用统一的成员名）
    BracketInfo round, square, curly;
    
    // 字符串跟踪
    struct {
        bool active = false;
        char quoteType = '\0';
        size_t startLine = 0;
        size_t startCol = 0;
        string startLineContent;
    } stringTracker;
    
    // 注释跟踪
    bool inMultiLineComment = false;

    for (size_t lineNum = 0; lineNum < lines.size(); ++lineNum) {
        const string& line = lines[lineNum];
        bool escaped = false;
        bool inSingleLineComment = false;

        for (size_t col = 0; col < line.size(); ++col) {
            char c = line[col];

            // 处理多行注释
            if (!stringTracker.active && !inSingleLineComment) {
                if (!inMultiLineComment && c == '/' && col + 1 < line.size() && line[col+1] == '*') {
                    inMultiLineComment = true;
                    col++;
                    continue;
                } else if (inMultiLineComment && c == '*' && col + 1 < line.size() && line[col+1] == '/') {
                    inMultiLineComment = false;
                    col++;
                    continue;
                }
            }

            if (inMultiLineComment) continue;

            // 处理单行注释
            if (!stringTracker.active && c == '/' && col + 1 < line.size() && line[col+1] == '/') {
                break;
            }

            /******* 字符串检测 *******/
            if (!stringTracker.active && !inMultiLineComment && (c == '"' || c == '\'')) {
                stringTracker.active = true;
                stringTracker.quoteType = c;
                stringTracker.startLine = lineNum + 1;
                stringTracker.startCol = col + 1;
                stringTracker.startLineContent = line;
            } 
            else if (stringTracker.active) {
                if (!escaped && c == stringTracker.quoteType) {
                    stringTracker.active = false;
                }
                escaped = (c == '\\' && !escaped);
            }
            /******* 字符串检测结束 *******/

            // 括号检测（使用统一的成员名）
            if (!stringTracker.active && !inMultiLineComment) {
                switch (c) {
                    case '(': 
                        round.count++;
                        round.lastOpenLine = lineNum + 1;
                        round.lastOpenCol = col + 1;
                        round.lastOpenLineContent = line;
                        break;
                    case ')':
                        if (--round.count < 0) {
                            error("SyntaxError", "多余的圆括号 ')'", 
                                 lineNum + 1, line, "error", fileName);
                            return;
                        }
                        break;
                    case '[': 
                        square.count++;
                        square.lastOpenLine = lineNum + 1;
                        square.lastOpenCol = col + 1;
                        square.lastOpenLineContent = line;
                        break;
                    case ']':
                        if (--square.count < 0) {
                            error("SyntaxError", "多余的方括号 ']'", 
                                 lineNum + 1, line, "error", fileName);
                            return;
                        }
                        break;
                    case '{': 
                        curly.count++;
                        curly.lastOpenLine = lineNum + 1;
                        curly.lastOpenCol = col + 1;
                        curly.lastOpenLineContent = line;
                        break;
                    case '}':
                        if (--curly.count < 0) {
                            error("SyntaxError", "多余的大括号 '}'", 
                                 lineNum + 1, line, "error", fileName);
                            return;
                        }
                        break;
                }
            }
        }

        // 行尾检查未闭合的字符串
        if (stringTracker.active) {
            error("SyntaxError", "字符串未闭合", 
                 stringTracker.startLine,
                 stringTracker.startLineContent,
                 "error", fileName);
            return;
        }
    }

    // 最终检查未闭合的括号（使用统一成员名）
    if (round.count > 0) {
        error("SyntaxError", "圆括号未闭合",
             round.lastOpenLine,
             round.lastOpenLineContent,
             "error", fileName);
    }
    if (square.count > 0) {
        error("SyntaxError", "方括号未闭合",
             square.lastOpenLine,
             square.lastOpenLineContent,
             "error", fileName);
    }
    if (curly.count > 0) {
        error("SyntaxError", "大括号未闭合",
             curly.lastOpenLine,
             curly.lastOpenLineContent,
             "error", fileName);
    }

    // 检查未闭合的多行注释
    if (inMultiLineComment) {
        error("SyntaxError", "多行注释未闭合",
             lines.size(),
             lines.back(),
             "error", fileName);
    }
}

