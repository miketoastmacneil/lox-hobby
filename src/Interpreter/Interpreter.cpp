

#include "ErrorHandling.hpp"
#include "Interpreter.hpp"


LiteralVariant Interpreter::Interpret(const Expr &expr) {
    return Evaluate(expr);
}

LiteralVariant Interpreter::VisitLiteral(const Literal &literal) {
    return literal.value;
}

LiteralVariant Interpreter::VisitGrouping(const Grouping &grouping) {
    return Evaluate(*grouping.expression);
}

LiteralVariant Interpreter::Evaluate(const Expr &expr) {
    return expr.accept(*this);
}

namespace {

struct UnaryVisitor {
    
    Token tok;
    // if it
    LiteralVariant operator()(double val) {
        if(tok.type == TokenType::MINUS) {
            return -val;
        } else {
            auto msg = "Unexpected token for unary operator with double right-hand-side.";
            lox::error(tok, msg);
            throw std::invalid_argument(msg);
        }
    }
    
    LiteralVariant operator()(bool val) {
        if(tok.type == TokenType::BANG) {
            return !val;
        } else {
            auto msg = "Unexpected unary operator for boolean RHS";
            lox::error(tok, msg);
            throw std::invalid_argument(msg);
        }
    }
    
    LiteralVariant operator()(const std::string& string) {  // String is truthy so return
        if(tok.type == TokenType::BANG) {
            return false;
        }
        auto msg = "Cannot have string as argument for unary operator minus";
        lox::error(tok, msg);
        throw std::invalid_argument(msg);
    }
    
    LiteralVariant operator()(const LoxNil& nil) {
        
        // Nil is not truthy, so negative that is true
        if(tok.type == TokenType::BANG) {
            return true;
        }
        auto msg = "Cannot have nil as argument for unary operator minus";
        lox::error(tok, msg);
        throw std::invalid_argument(msg);
    }
    
    
};

bool TokenIsValidUnaryOp(const Token& op) {
    return (op.type == TokenType::MINUS) || (op.type == TokenType::BANG);
}

}

LiteralVariant Interpreter::VisitUnary(const Unary &expr) {
    auto right = Evaluate(*expr.right);
    if(!TokenIsValidUnaryOp(expr.op)) {
        lox::error(expr.op, "Token is invalid as a unary operator");
        throw std::invalid_argument("Token is an invalid unary operator");
    }
    return std::visit(UnaryVisitor{expr.op}, right);
}

namespace {

template<typename T, class Variant>
bool IsType(const Variant& v) {
    return std::holds_alternative<T>(v);
}

template<typename T>
bool AreType(const LiteralVariant& left, const LiteralVariant& right) {
    return IsType<T>(left) && IsType<T>(right);
}

bool IsEqual(const LiteralVariant& left, const LiteralVariant& right) {
    
    // Handle null
    if(AreType<LoxNil>(left, right)) {
        return true;
    }
    
    if(IsType<LoxNil>(left) && (!IsType<LoxNil>(right))) {
        return false;
    }
    
    if(!IsType<LoxNil>(left) && IsType<LoxNil>(right)) {
        return false;
    }
    
    // Then only compare like types.
    if(AreType<double>(left, right)) {
        return std::get<double>(left) == std::get<double>(right);
    }
    
    if(AreType<std::string>(left, right)){
        return std::get<std::string>(left) == std::get<std::string>(right);
    }
    
    if(AreType<bool>(left, right)) {
        return std::get<bool>(left) == std::get<bool>(right);
    }
    
    // Anything else is false.
    return false;
}

}

LiteralVariant Interpreter::VisitBinary(const Binary &expr) {
    auto left = Evaluate(*expr.left);
    auto right = Evaluate(*expr.right);
    
    auto report_non_double = [](const Binary& expr) {
        lox::error(expr.op, "Inputs for binary expression need to be doubles.");
        throw std::invalid_argument("Inputs for binary expression need to be doubles.");
    };
    
    switch (expr.op.type) {
        case TokenType::BANG:
            return IsEqual(left, right);
            break;
        case TokenType::BANG_EQUAL:
            return !IsEqual(left, right);
        case TokenType::GREATER:
            if(AreType<double>(left, right)) {
                return std::get<double>(left) > std::get<double>(right);
            }
            report_non_double(expr);
            break;
        case TokenType::GREATER_EQUAL:
            if(AreType<double>(left, right)) {
                return std::get<double>(left) >= std::get<double>(right);
            }
            report_non_double(expr);
            break;
        case TokenType::LESS:
            if(AreType<double>(left, right)) {
                return std::get<double>(left) < std::get<double>(right);
            }
            report_non_double(expr);
            break;
        case TokenType::LESS_EQUAL:
            if(AreType<double>(left, right)) {
                return std::get<double>(left) <= std::get<double>(right);
            }
            report_non_double(expr);
            break;
        case TokenType::MINUS:
            if(AreType<double>(left, right)) {
                return std::get<double>(left) - std::get<double>(right);
            }
            report_non_double(expr);
            break;
        case TokenType::SLASH:
            if(AreType<double>(left, right)) {
                return std::get<double>(left) / std::get<double>(right);
            }
            report_non_double(expr);
            break;
        case TokenType::STAR:
            if(AreType<double>(left, right)) {
                return std::get<double>(left)*std::get<double>(right);
            }
            report_non_double(expr);
            break;
        case TokenType::PLUS:
            if(AreType<std::string>(left, right)) {
                return std::get<std::string>(left) + std::get<std::string>(right);
            }
            if(AreType<double>(left, right)) {
                return std::get<double>(left) + std::get<double>(right);
            }
            // Otherwise report an error
            lox::error(expr.op.line, "Arguments for addition need to be strings or double");
            break;
        default:
            break;
    }
        
    // Unreachable.
    return LiteralVariant(LoxNil());
    
}


