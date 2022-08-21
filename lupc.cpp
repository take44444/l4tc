#include "bits/stdc++.h"
#include "lupc.hpp"

int main() {
  std::ostringstream ost;
  ost << std::cin.rdbuf();
  std::string source = ost.str();
  tokenizer::Token *token_list = tokenizer::tokenize(source);
  parser::Error error = parser::Error("", "", NULL);
  std::shared_ptr<parser::AST> ast = parser::parse(&token_list, error);
  if (!ast) {
    // print_tokens(token_list);
    std::cerr << error.get_error_string() << std::endl;
  } else {
    parser::print_ast(ast);
  }
  // preprocess(token_list);
  // AST *ast = parse(token_list);
  // SymbolEntry *ctx = analyze(ast);
  // string assembly = generate(ast, ctx);
}