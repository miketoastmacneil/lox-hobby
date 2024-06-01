
#pragma once

#include <any>
#include <ostream>
#include <string>

enum class TokenType {
  // Single Character Tokens
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,

  // One or Two Character Tokens
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  // Literals
  IDENTIFIER,
  STRING,
  NUMBER,

  // Keywords
  AND,
  CLASS,
  ELSE,
  FALSE,
  FUNC,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,

  LOX_EOF
};

std::ostream& operator<<(std::ostream& os, const TokenType type);

class Token {

public:
  Token(TokenType type, std::string lexeme, std::any literal, int line)
      : type_{type}, lexeme_{lexeme}, literal_{literal}, line_{line} {}

        // Ostream operator overload (as a member function)
    friend std::ostream& operator<<(std::ostream& os, const Token& token);

private:
  TokenType type_;
  std::string lexeme_;
  std::any literal_;
  int line_;
};