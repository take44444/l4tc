#include "lupc.h"

int main() {
  ostringstream ost;
  ost << cin.rdbuf();
  string source = ost.str();
  Token *token_list = tokenize(source);
  // parse(&token_list);
  print_tokens(token_list);
  // preprocess(token_list);
  // AST *ast = parse(token_list);
  // SymbolEntry *ctx = analyze(ast);
  // string assembly = generate(ast, ctx);
}