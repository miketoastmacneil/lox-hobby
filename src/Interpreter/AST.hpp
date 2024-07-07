

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


class ExprVisitor {
public:
    virtual LiteralVariant VisitLiteral(const Literal& literal) = 0;
    virtual LiteralVariant VisitGrouping(const Grouping& grouping) = 0;
    virtual LiteralVariant VisitUnary(const Unary& unary) = 0;
    virtual LiteralVariant VisitBinary(const Binary& binary) = 0;
    virtual ~ExprVisitor() = default;
};

struct Expr  {
virtual LiteralVariant accept(ExprVisitor& visitor) const = 0;
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

  inline LiteralVariant accept(ExprVisitor& visitor) const override { return visitor.VisitLiteral(*this);
    }
};

struct Grouping : Expr {
    ExprPtr expression;
    Grouping(ExprPtr expr) : expression{std::move(expr)} {}
    
    inline LiteralVariant accept(ExprVisitor& visitor) const override {
        return visitor.VisitGrouping(*this);
    }
};

struct Unary : Expr {
  ExprPtr right;
  Token op;

  Unary(ExprPtr in_right, Token &&token)
      : right{std::move(in_right)}, op{std::move(token)} {}

    inline LiteralVariant accept(ExprVisitor& visitor) const override { return visitor.VisitUnary(*this);
    }
};

struct Binary : Expr {
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;
  Token op;

  Binary(ExprPtr left, ExprPtr right, Token &&token)
      : left(std::move(left)), right{std::move(right)}, op{std::move(token)} {}

    LiteralVariant accept(ExprVisitor& visitor) const override { return visitor.VisitBinary(*this);
    }
};
