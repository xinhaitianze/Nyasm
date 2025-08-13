#pragma once
#include <vector>
#include <string>
#include <unordered_set>
#include <cctype>

// 定义关键字集合
const std::unordered_set<std::string> KEYWORDS = {
    "print", "if", "elif", "else", "for", "while", 
    "input", "return", "def", "class", "import",
    "int", "str", "list", "bool", "float",
    "true", "false", "null", "break", "continue"
};

// 定义符号集合（单独分割）
const std::unordered_set<char> SYMBOLS = {
    ';', '(', ')', '{', '}', '[', ']', 
    '+', '-', '*', '/', '=', '<', '>', '!', 
    ',', '.', ':', '&', '|', '%', '^'
};

// Token 类型（可选）
enum class TokenType {
    KEYWORD,    // 关键字（print, if, else...）
    IDENTIFIER, // 变量名（abc, x123...）
    SYMBOL,     // 符号（;, (, ), ...）
    LITERAL,    // 字面量（"hello", 123, 3.14...）
    WHITESPACE  // 空格、换行等
};

// Token 结构体（可选）
struct Token {
    std::string value;
    TokenType type;
};

// 主 Tokenizer 函数
std::vector<std::string> tokenize_code(const std::string& code) {
    std::vector<std::string> tokens;
    std::string current_token;
    bool in_string = false;  // 是否在字符串字面量中（如 "hello"）
    char string_quote = '\0'; // 当前字符串的引号类型（' or "）

    for (size_t i = 0; i < code.size(); i++) {
        char c = code[i];

        // 处理字符串字面量（"..." 或 '...'）
        if (c == '"' || c == '\'') {
            if (!in_string) {
                // 开始字符串
                in_string = true;
                string_quote = c;
                if (!current_token.empty()) {
                    tokens.push_back(current_token);
                    current_token.clear();
                }
                current_token += c;
            } else if (c == string_quote) {
                // 结束字符串
                current_token += c;
                tokens.push_back(current_token);
                current_token.clear();
                in_string = false;
            } else {
                // 字符串内的其他引号（如 "It's"）
                current_token += c;
            }
            continue;
        }

        // 如果在字符串内，直接添加字符
        if (in_string) {
            current_token += c;
            continue;
        }

        // 处理符号（单独分割）
        if (SYMBOLS.find(c) != SYMBOLS.end()) {
            if (!current_token.empty()) {
                tokens.push_back(current_token);
                current_token.clear();
            }
            tokens.push_back(std::string(1, c)); // 符号作为独立 Token
            continue;
        }

        // 处理空格（分割 Token）
        if (isspace(c)) {
            if (!current_token.empty()) {
                tokens.push_back(current_token);
                current_token.clear();
            }
            continue;
        }

        // 否则，继续构建当前 Token
        current_token += c;
    }

    // 添加最后一个 Token
    if (!current_token.empty()) {
        tokens.push_back(current_token);
    }

    return tokens;
}