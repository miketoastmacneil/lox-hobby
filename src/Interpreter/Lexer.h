#pragma once

#include <iostream>
#include <string>

class Lexer {
public:
  Lexer() = default;

  std::string getNextLine() {
    std::string line;
    std::getline(std::cin, line);
    return line;
  }
};
