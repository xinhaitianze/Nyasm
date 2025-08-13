#pragma once
#include <fstream>
#include <string>
#include <sstream>


std::string read(const char *FileName) {
    std::string Code, line;
    std::ifstream inputFile(FileName);

    // Return error
    if (!inputFile.is_open()) {
        return "!Error";
    }
    else {

        // Read code in file
        std::stringstream buffer;
        while (std::getline(inputFile, line)) {
            buffer << line << "\n";
        }
        Code = buffer.str();
        //Code = Code + "*!*[File " + FileName + " End]*!*";

        inputFile.close();

        return Code;
    }
}