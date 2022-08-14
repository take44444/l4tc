#include "lupc.h"

int main() {
  ostringstream ost;
  ost << cin.rdbuf();
  string source = ost.str();
  TokenNode *token_list = tokenize(source);
  print_tokens(token_list);
  // preprocess(token_list);
  // ASTNode *ast = parse(token_list);
  // SymbolEntry *ctx = analyze(ast);
  // string assembly = generate(ast, ctx);
}