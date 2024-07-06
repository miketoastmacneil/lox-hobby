#include <filesystem>
#include <iostream>
#include <set>
#include <string>

#include "interpreter/Scanner.hpp"
#include "interpreter/Parser.hpp"
#include "interpreter/Interpreter.hpp"

namespace fs = std::filesystem;

const std::set<std::string> exit_keys = {"exit", "exit()"};

namespace lox {


}

void Run(const std::string &src) {

  auto scanner = Scanner(src);
  auto tokens = scanner.ScanTokens();
    Parser parser(tokens);
    auto expr = parser.Parse();

  for (auto token : tokens) {
    std::cout << token << std::endl;
  }
}

void Repl() {
  std::string input;

  while (true) {
    std::cout << ">> ";            // Display the REPL prompt
    std::getline(std::cin, input); // Read the entire line of input

    if (exit_keys.contains(input)){
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
