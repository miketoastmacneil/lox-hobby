
#include <filesystem>
#include <iostream>
#include <set>
#include <string>

#include "Interpreter/Lexer.h"
#include "Interpreter/Scanner.h"

namespace fs = std::filesystem;

const std::set<std::string> exit_keys = {"exit", "exit()"};

void Run(const std::string &src) {

  auto scanner = Scanner(src);
  auto tokens = scanner.ScanTokens();

  for (auto token : tokens) {
    std::cout << token << std::endl;
  }
}

void Repl() {
  std::string input;

  while (true) {
    std::cout << ">> ";            // Display the REPL prompt
    std::getline(std::cin, input); // Read the entire line of input

    auto pos = exit_keys.find(input);
    if (pos != exit_keys.end()) {
      break;
    }

    Run(input);
  }

  std::cout << "Exiting REPL..." << std::endl;
}

void RunFile(const fs::path &path) {}
int main(int argc, char *argv[]) {
  fs::path filename; // Use fs::path for the filename
  if (argc > 1) {
    filename = argv[1];
    if (filename.extension() != ".lox") { // Check extension directly
      std::cerr << "Error: Input file must have a '.lox' extension."
                << std::endl;
      return 1;
    }
    if (filename.empty()) {
      std::cerr << "Error: Input lox file is empty" << std::endl;
    }
    RunFile(filename);
  } else {
    Repl();
  }

  return 0;
}
