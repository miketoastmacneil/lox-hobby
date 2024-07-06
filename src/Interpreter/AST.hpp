

#pragma once

#include <any>
#include <concepts>
#include <memory>
#include <variant>

#include "Tokens.hpp"

struct Literal;
struct Grouping;
struct Unary;
struct Binary;


template <typename T>
concept ExprVisitor = requires(T t, const Literal &l, const Grouping &gr,
                               const Unary &un, const Binary &bin) {
  { t.Visit(l) } -> std::same_as<LiteralVariant>;
  { t.visit(gr) } -> std::same_as<void>;
  { t.visit(un) } -> std::same_as<void>;
  { t.visit(bin) } -> std::same_as<void>;
};

struct Expr {
  virtual ~Expr() = default;
};

using ExprPtr = std::unique_ptr<Expr>;

struct Literal : Expr {
  std::variant<double, std::string, bool, LoxNil> value;

  Literal(const LiteralVariant& variant):value{variant}{}
  Literal(double val) : value{val} {}
  Literal(std::string val) : value{val} {}
  Literal(bool val) : value{val} {}
  Literal(const LoxNil &val) : value{val} {}

  void accept(ExprVisitor auto &visitor) { visitor->visit(*this); }
};

struct Grouping : Expr {
  ExprPtr expression;
  Grouping(ExprPtr expr) : expression{std::move(expr)} {}

  template<typename RetT, typename VisitT>
  RetT accept(VisitT visitor) { return visitor.Visit(*this); }
};

struct Unary : Expr {
  ExprPtr right;
  Token op;

  Unary(ExprPtr in_right, Token &&token)
      : right{std::move(in_right)}, op{std::move(token)} {}

  void accept(ExprVisitor auto &visitor) { visitor->visit(*this); }
};

struct Binary : Expr {
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;
  Token op;

  Binary(ExprPtr left, ExprPtr right, Token &&token)
      : left(std::move(left)), right{std::move(right)}, op{std::move(token)} {}

  void accept(ExprVisitor auto &visitor) { visitor.visit(*this); }
};
