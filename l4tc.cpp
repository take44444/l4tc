#include "bits/stdc++.h"
#include "l4tc.hpp"

int main() {
  std::ostringstream ost;
  ost << std::cin.rdbuf();
  std::string source = ost.str();
  tokenizer::Token *token_list = tokenizer::tokenize(source);
  parser::PError perror = parser::PError("", "", NULL);
  std::shared_ptr<parser::AST> ast = parser::parse(&token_list, perror);
  if (!ast) {
    std::cerr << perror.get_error_string() << std::endl;
    return 1;
  }
  generator::GError gerror = generator::GError("", NULL);
  std::string code = generator::generate(ast, gerror);
  if (code.size() == 0) {
    std::cerr << gerror.get_error_string() << std::endl;
    return 1;
  }
  std::cout << code;
  return 0;
}