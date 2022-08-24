#include "./generator.hpp"
#include "../parser/parser.hpp"

namespace generator {
  std::shared_ptr<EvalType> create_type(std::shared_ptr<parser::ASTDeclarator> d, std::shared_ptr<EvalType> base_type) {
    // TODO: pointer
    return base_type;
  }

  std::shared_ptr<EvalType> create_base_type(std::shared_ptr<parser::ASTTypeSpec> n) {
    // TODO static, const
    switch (n->op->type)
    {
    case tokenizer::KwNum:
      return std::make_shared<TypeNum>();
    case tokenizer::KwVoid:
      return std::make_shared<TypeVoid>();
    default:
      break;
    }
    assert(false);
    return nullptr;
  }

  void generator_sub(std::shared_ptr<parser::AST> ast, std::shared_ptr<SymbolEntrys> se, std::string &code) {
    // declaration-spec simple-declarators
    if (typeid(*ast) == typeid(parser::ASTExternalDeclaration)) {
      std::shared_ptr<parser::ASTExternalDeclaration> n = std::dynamic_pointer_cast<parser::ASTExternalDeclaration>(ast);
      std::shared_ptr<EvalType> base_type = create_base_type(n->declaration_spec);
      for (std::shared_ptr<parser::ASTDeclarator> d: n->declarators) {
        se->add_global_var(std::string(d->op->sv), create_type(d, base_type));
      }
      return;
    }
    if (typeid(*ast) == typeid(parser::ASTFuncDef)) {
      std::shared_ptr<parser::ASTFuncDef> n = std::dynamic_pointer_cast<parser::ASTFuncDef>(ast);
      std::vector<std::shared_ptr<EvalType>> type_args;
      for (
        std::shared_ptr<parser::ASTSimpleDeclaration> d: n->declaration->declarator->args
      ) {
        type_args.push_back(create_base_type(d->type_spec));
      }
      se->add_func(
        std::string(n->declaration->declarator->declarator->op->sv),
        create_type(
          n->declaration->declarator->declarator,
          create_base_type(n->declaration->type_spec)
        ),
        type_args
      );
      generator_sub(n->body, se, code);
    }
    if (typeid(*ast) == typeid(parser::ASTDeclaration)) {
      std::shared_ptr<parser::ASTDeclaration> n = std::dynamic_pointer_cast<parser::ASTDeclaration>(ast);
      std::shared_ptr<EvalType> base_type = create_base_type(n->declaration_spec);
      for (std::shared_ptr<parser::ASTDeclarator> d: n->declarators) {
        se->add_local_var(std::string(d->op->sv), create_type(d, base_type));
      }
      return;
    }
    if (typeid(*ast) == typeid(parser::ASTCompoundStmt)) {
      std::shared_ptr<parser::ASTCompoundStmt> n = std::dynamic_pointer_cast<parser::ASTCompoundStmt>(ast);
      se->start_scope();
      for (std::shared_ptr<parser::AST> stmt: n->items) {
        generator_sub(stmt, se, code);
      }
      se->end_scope();
    }
    if (typeid(*ast) == typeid(parser::ASTExprStmt)) {
      std::shared_ptr<parser::ASTExprStmt> n = std::dynamic_pointer_cast<parser::ASTExprStmt>(ast);
      generator_sub(n->expr, se, code);
      code += "pop r10\n"; // pop the value that need not be evaluate
    }
    if (typeid(*ast) == typeid(parser::ASTReturnStmt)) {
      std::shared_ptr<parser::ASTReturnStmt> n = std::dynamic_pointer_cast<parser::ASTReturnStmt>(ast);
      generator_sub(n->expr, se, code);
      code += "pop rax\n"; // set return value
      code += "mov rsp, rbp\n";
      code += "pop rbp\n";
      code += "ret\n";
    }
  }


  std::string generator(std::shared_ptr<parser::AST> ast) {
    std::string ret;
    std::shared_ptr<SymbolEntrys> symbol_entrys = std::make_shared<SymbolEntrys>();
    generator_sub(ast, symbol_entrys, ret);
    return ret;
  }
}