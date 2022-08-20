#include "lupc.h"

int main() {
  ostringstream ost;
  ost << cin.rdbuf();
  string source = ost.str();
  Token *token_list = tokenize(source);
  Error error = Error("");
  shared_ptr<AST> ast = parse(&token_list, error);
  if (!ast) {
    print_tokens(token_list);
    printf("%s\n", &error.err_str[0]);
  } else {
    print_ast(ast);
  }
  // preprocess(token_list);
  // AST *ast = parse(token_list);
  // SymbolEntry *ctx = analyze(ast);
  // string assembly = generate(ast, ctx);
}