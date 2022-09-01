#include "bits/stdc++.h"
#include "l4tc.hpp"

int main() {
  std::ostringstream ost;
  ost << std::cin.rdbuf();
  std::string source = ost.str();
  tokenizer::Token *token_list = tokenizer::tokenize(source);
  parser::Error error = parser::Error("", "", NULL);
  std::shared_ptr<parser::AST> ast = parser::parse(&token_list, error);
  if (!ast) {
    std::cerr << error.get_error_string() << std::endl;
    parser::print_ast(ast);
  } else {
    std::cout << generator::generate(ast);
  }
}