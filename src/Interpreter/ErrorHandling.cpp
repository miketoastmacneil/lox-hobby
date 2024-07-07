

#include "ErrorHandling.hpp"
#include <iostream>

namespace lox {

void report(int line, std::string where, std::string msg) {
    std::cout << "[line " << line << "] Error " << where
    << ": " << msg;
    had_error = true;
}

void error(int line, std::string msg) {
    report(line, "", msg);
}

void error(const Token& token, const std::string& msg) {
    if(token.type == TokenType::LOX_EOF){
        report(token.line, "at end", msg);
    } else {
        report(token.line, "at " + token.lexeme, msg);
    }
}
}
