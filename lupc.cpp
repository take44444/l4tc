#include "lupc.h"

int main() {
  ostringstream ost;
  ost << cin.rdbuf();
  string source = ost.str();
  Token *token_list = tokenize(source);
  Error error = Error("");
  if (!parse(&token_list, error)) {
    print_tokens(token_list);
    printf("%s\n", &error.err_str[0]);
  }
  // preprocess(token_list);
  // AST *ast = parse(token_list);
  // SymbolEntry *ctx = analyze(ast);
  // string assembly = generate(ast, ctx);
}