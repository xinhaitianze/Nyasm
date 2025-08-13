#include <vector>
#include <string>  
#include "./open.h"
#include <iostream>


// 去除字符串两端空白字符
std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";

    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

// 分割字符串为行
std::vector<std::string> split_lines(const std::string& str) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = str.find('\n');

    while (end != std::string::npos) {
        result.push_back(trim(str.substr(start, end - start)));  // 把两头空格去了
        start = end + 1;
        end = str.find('\n', start);
    }

    // 添加最后一行
    result.push_back(trim(str.substr(start)));
    return result;
}