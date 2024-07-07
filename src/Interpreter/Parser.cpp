
#include "ErrorHandling.hpp"
#include "Parser.hpp"
#include <memory>

namespace {
ExprPtr MakeBinary(ExprPtr left, ExprPtr right, Token &&op) {
  return std::make_unique<Binary>(std::move(left), std::move(right),
                                  std::move(op));
}

ExprPtr MakeUnary(ExprPtr right, Token &&op) {
  return std::make_unique<Unary>(std::move(right), std::move(op));
}

} // namespace

ExprPtr Parser::Parse() { return Expression(); }

ExprPtr Parser::Expression() { return Equality(); }

ExprPtr Parser::Equality() {
  ExprPtr expr = Comparison();

  while (Match({TokenType::BANG, TokenType::BANG_EQUAL})) {
    auto token = Previous();
    ExprPtr right = Comparison();
    expr = MakeBinary(std::move(expr), std::move(right), std::move(token));
  }
  return std::move(expr);
}

ExprPtr Parser::Comparison() {
  ExprPtr expr = Term();

  while (Match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS,
                TokenType::LESS_EQUAL})) {
    auto op = Previous();
    ExprPtr right = Term();
    expr = MakeBinary(std::move(expr), std::move(right), std::move(op));
  }

  return expr;
}

ExprPtr Parser::Term() {
  auto expr = Factor();

  while (Match({TokenType::MINUS, TokenType::PLUS})) {
    auto op = Previous();
    ExprPtr right = Factor();
    expr = MakeBinary(std::move(expr), std::move(right), std::move(op));
  }

  return expr;
}

ExprPtr Parser::Factor() {
  auto expr = Unary();

  while (Match({TokenType::SLASH, TokenType::STAR})) {
    auto op = Previous();
    ExprPtr right = Unary();
    expr = MakeBinary(std::move(expr), std::move(right), std::move(op));
  }
  return expr;
}

ExprPtr Parser::Unary() {
  if (Match({TokenType::BANG, TokenType::MINUS})) {
    auto op = Previous();
    ExprPtr right = Unary();
    return MakeUnary(std::move(right), std::move(op));
  }

  return Primary();
}

ExprPtr Parser::Primary() {

  auto MakeLiteral = [](const auto &type) {
    return std::make_unique<Literal>(type);
  };

  if (Match({TokenType::FALSE})) {
    return MakeLiteral(false);
  }
  if (Match({TokenType::TRUE})) {
    return MakeLiteral(true);
  }
  if (Match({TokenType::NIL})) {
    return MakeLiteral(LoxNil{});
  }

  if (Match({TokenType::NUMBER})) {
    return MakeLiteral(Previous().literal);
  }

  if (Match({TokenType::STRING})) {
    return MakeLiteral(Previous().literal);
  }

  if (Match({TokenType::LEFT_PAREN})) {
    auto expr = Expression();
    Consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");
    return std::make_unique<Grouping>(std::move(expr));
  }
}

bool Parser::Match(const std::vector<TokenType> &types) {
  for (auto type : types) {
    if (Check(type)) {
      Advance();
      return true;
    }
  }
}

bool Parser::Check(TokenType type) const {
  if (IsAtEnd()) {
    return false;
  }
  return Peek().type == type;
}

Token Parser::Advance() {
  if (!IsAtEnd()) {
    current_++;
    return Previous();
  }
}

bool Parser::IsAtEnd() const { return Peek().type == TokenType::LOX_EOF; }

Token Parser::Previous() const { return tokens_[current_ - 1]; }

Token Parser::Peek() const { return tokens_[current_]; }

Token Parser::Consume(TokenType type, std::string msg) {
    
    // Need to hook this up to the error reporting. 
  if (Check(type))
    return Advance();
  lox::error(Peek(), msg);
  throw std::invalid_argument(msg);
}
