#include <concepts>
#include <iostream>
#include <memory>

struct Double;
struct Add;

// There are only two at the moment.

template <typename T>
concept ExprVisitor = requires(T t, const Double &d, const Add &add) {
  { t.visit(d) } -> std::same_as<void>;   // Ensure return type is void
  { t.visit(add) } -> std::same_as<void>; // Ensure return type is void
};

struct Expr {
  virtual ~Expr() = default;
};

struct Double : Expr {
  double value;
  Double(double val) : value{val} {}

  void accept(ExprVisitor auto visitor) { visitor->visit(*this); }
};

struct Add : Expr {
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;

  Add(Expr &&left, Expr &&right)
      : left{std::make_unique<Expr>(std::move(left))},
        right{std::make_unique<Expr>(std::move(right))} {}

  void accept(ExprVisitor auto visitor) { visitor.visit(*this); }
};

int main() {

  auto otherAdd = Add{Double{2.0}, Double{1.0}};

  auto expr = Add{Double{2.0}, Add{Double{4.0}, std::move(otherAdd)}};
}
