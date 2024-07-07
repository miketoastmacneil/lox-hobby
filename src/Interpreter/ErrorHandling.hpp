
#pragma once

#include <string>
#include "Tokens.hpp"

namespace lox {
static bool had_error = false;

void report(int line, std::string where, std::string msg);

void error(int line, std::string msg);

void error(const Token& token, const std::string& msg);
}

