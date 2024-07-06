

#pragma once

#include "AST.hpp"
#include "Tokens.hpp"

class Parser {

public:
  Parser(const std::vector<Token> &tokens) : tokens_{tokens}, current_{0} {}

  ExprPtr Parse();

private:
  ExprPtr Expression();
  ExprPtr Equality();
  ExprPtr Comparison();
  ExprPtr Term();
  ExprPtr Factor();
  ExprPtr Unary();
  ExprPtr Primary();

  bool Match(const std::vector<TokenType> &types);
  bool Check(TokenType type) const;
  Token Advance();
  bool IsAtEnd() const;
  Token Previous() const;
  Token Peek() const;
  Token Consume(TokenType type, std::string msg);

  std::vector<Token> tokens_;
  int current_;
};
