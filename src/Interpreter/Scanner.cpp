
#include <ctype.h>
#include <iostream>

#include "Interpreter/Scanner.h"

std::vector<Token> Scanner::ScanTokens() {
    while(!AtEnd()) {
        start_ = current_;
        ScanToken();
    }

    tokens_.emplace_back(Token{TokenType::LOX_EOF, "", LoxNil{}, line_});
    return tokens_;
}

void Scanner::ScanToken() {
    auto c = Advance();
    switch(c) {
        case '(': 
            AddToken(TokenType::LEFT_PAREN); break;
        case ')':
            AddToken(TokenType::RIGHT_PAREN); break;
        case '{': AddToken(TokenType::LEFT_BRACE); break;
        case '}': AddToken(TokenType::RIGHT_BRACE); break;
        case ',': AddToken(TokenType::COMMA); break;
        case '.': AddToken(TokenType::DOT); break;
        case '-': AddToken(TokenType::MINUS); break;
        case '+': AddToken(TokenType::PLUS); break;
        case ';': AddToken(TokenType::SEMICOLON); break;
        case '*': AddToken(TokenType::STAR); break;
        case '!':
            AddToken(Match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            AddToken(Match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
            AddToken(Match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>':
            AddToken(Match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        case '/':
            if(Match('/')) {
                // Comment runs till the end of the line
                while(Peek()!='\n' && !AtEnd()) Advance();
            } else {
                AddToken(TokenType::SLASH);
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            line_++; break;
        case '"':
            String(); break;
        default:
            if(IsDigit(c)) {
                Number();
            } else if ( IsAlpha(c) ) {
                Identifier();
            } else {
                std::cout << "Unrecognized Token: " << c << std::endl;
            }
    }
}
