
#pragma once

#include <ostream>
#include <string>

struct LoxNil {};

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

using LiteralVariant = std::variant<double, std::string, bool, LoxNil>;

struct LiteralPrinter {
    
    inline std::string operator()(const std::string& i) {
        return i;
    }
    
    inline std::string operator()(double val) {
        return std::to_string(val);
    }
    
    inline std::string operator()(bool val) {
        return std::to_string(val);
    }
    
    inline std::string operator()(const LoxNil& nil) {
        return "Nil";
    }
};

std::ostream &operator<<(std::ostream &os, const TokenType type);

struct Token {

public:
  Token(TokenType type, std::string lexeme, LiteralVariant literal, int line)
      : type{type}, lexeme{lexeme}, literal{literal}, line{line} {}

  // Ostream operator overload (as a member function)
  friend std::ostream &operator<<(std::ostream &os, const Token &token);

  TokenType type;
  std::string lexeme;
  LiteralVariant literal;
  int line;
};
