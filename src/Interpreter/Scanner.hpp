

#pragma once

#include <cctype>
#include <map>
#include <optional>
#include <vector>

#include "Tokens.hpp"

namespace scanner::detail {

const static std::map<std::string, TokenType> kKeywords{
    {"and", TokenType::AND},       {"class", TokenType::CLASS},
    {"else", TokenType::ELSE},     {"false", TokenType::FALSE},
    {"for", TokenType::FOR},       {"func", TokenType::FUNC},
    {"if", TokenType::IF},         {"nil", TokenType::NIL},
    {"or", TokenType::OR},         {"print", TokenType::PRINT},
    {"return", TokenType::RETURN}, {"super", TokenType::SUPER},
    {"this", TokenType::THIS},     {"true", TokenType::TRUE},
    {"var", TokenType::VAR},       {"while", TokenType::WHILE}};

}

class Scanner {
public:
  Scanner(const std::string &src) : src_{src} {}
  std::vector<Token> ScanTokens();

private:
  void ScanToken();

  inline bool AtEnd() const { return current_ >= src_.size(); }

  inline char Advance() { return src_.at(current_++); }

  inline void AddToken(TokenType type) { AddToken(type, LoxNil{}); }

  inline void AddToken(TokenType type, LiteralVariant literal) {
    auto text = src_.substr(start_, current_ - start_);
    tokens_.emplace_back(Token(type, text, literal, line_));
  }

  inline bool Match(char expected) {
    if (AtEnd()) {
      return false;
    }
    if (src_.at(current_) != expected) {
      return false;
    }

    current_++;
    return true;
  }

  inline char Peek() {
    if (AtEnd())
      return '\0';
    return src_.at(current_);
  }

  inline char PeekNext() {
    if (current_ + 1 >= src_.size())
      return '\0';
    return src_.at(current_ + 1);
  }

  inline void String() {
    while (Peek() != '"' && !AtEnd()) {
      if (Peek() == '\n')
        line_++;
      Advance();
    }

    Advance(); // Handle the closing '""

    auto val = src_.substr(start_ + 1, current_ - start_ - 1);
    AddToken(TokenType::STRING, val);
  }

  inline void Number() {
    while (IsDigit(Peek()))
      Advance();
    if (Peek() == '.' && IsDigit(PeekNext()))
      Advance();
    while (IsDigit(Peek()))
      Advance();
    AddToken(TokenType::NUMBER, stod(src_.substr(start_, current_ - start_)));
  }

  inline void Identifier() {
    while (IsAlphaNumeric(Peek()))
      Advance();

    auto text = src_.substr(start_, current_ - start_);
    auto pos = scanner::detail::kKeywords.find(text);
    if (pos == scanner::detail::kKeywords.end()) {
      AddToken(TokenType::IDENTIFIER);
      return;
    }
    AddToken(pos->second);
  }

  inline bool IsAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
  }

  inline bool IsDigit(char c) { return (c >= '0') && (c <= '9'); }

  inline bool IsAlphaNumeric(char c) { return IsAlpha(c) || IsDigit(c); }

  std::string src_;
  int start_ = 0;
  int current_ = 0;
  int line_ = 1;

  std::vector<Token> tokens_;
};
