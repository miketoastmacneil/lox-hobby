

#pragma once
#include "AST.hpp"

class Interpreter: public ExprVisitor {
public:
    
    LiteralVariant Interpret(const Expr& expr);
    
private:
    
    LiteralVariant VisitLiteral(const Literal& literal);
    
    LiteralVariant VisitGrouping(const Grouping& grouping);
    
    LiteralVariant Evaluate(const Expr& expr);
    
    LiteralVariant VisitUnary(const Unary& expr);
    
    LiteralVariant VisitBinary(const Binary& expr);
};
