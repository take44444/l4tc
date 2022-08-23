#include "./analyzer.hpp"
#include "../parser/parser.hpp"

namespace analyzer {
  tokenizer::TokenType get_base_type(std::shared_ptr<parser::ASTTypeSpec> n) {
    return n->op->type;
  }

  void analyze_sub(std::shared_ptr<parser::AST> ast, std::shared_ptr<SymbolEntrys> se) {
    // declaration-spec simple-declarators
    if (typeid(*ast) == typeid(parser::ASTExternalDeclaration)) {
      std::shared_ptr<parser::ASTExternalDeclaration> n = std::dynamic_pointer_cast<parser::ASTExternalDeclaration>(ast);
      tokenizer::TokenType base_type = get_base_type(n->declaration_spec);
      for (std::shared_ptr<parser::ASTDeclarator> d: n->declarators) {
        se->
      }
    }
  }


  std::shared_ptr<SymbolEntrys> analyze(std::shared_ptr<parser::AST> ast) {
    std::shared_ptr<SymbolEntrys> symbol_entrys = std::make_shared<SymbolEntrys>();
    analyze_sub(ast, symbol_entrys);
  }
}