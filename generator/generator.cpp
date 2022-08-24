#include "./generator.hpp"

namespace generator {
  std::shared_ptr<EvalType> create_type(std::shared_ptr<ASTDeclarator> d, std::shared_ptr<EvalType> base_type) {
    // TODO: pointer
    return base_type;
  }

  std::shared_ptr<EvalType> create_base_type(std::shared_ptr<ASTTypeSpec> n) {
    // TODO static, const
    switch (n->op->type)
    {
    case KwNum:
      return std::make_shared<TypeNum>();
    case KwVoid:
      return std::make_shared<TypeVoid>();
    default:
      break;
    }
    assert(false);
    return nullptr;
  }

  void generator_sub(std::shared_ptr<AST> ast, std::shared_ptr<SymbolEntrys> se, std::string &code) {
    if (typeid(*ast) == typeid(ASTTranslationUnit)) {
      std::shared_ptr<ASTTranslationUnit> n = std::dynamic_pointer_cast<ASTTranslationUnit>(ast);
      code += ".intel_syntax noprefix\n"; // use intel syntax
      code += ".text\n";
      for (std::shared_ptr<AST> d: n->external_declarations) {
        generator_sub(d, se, code);
      }
    }
    // declaration-spec simple-declarators
    if (typeid(*ast) == typeid(ASTExternalDeclaration)) {
      std::shared_ptr<ASTExternalDeclaration> n = std::dynamic_pointer_cast<ASTExternalDeclaration>(ast);
      std::shared_ptr<EvalType> base_type = create_base_type(n->declaration_spec);
      for (std::shared_ptr<ASTDeclarator> d: n->declarators) {
        se->add_global_var(std::string(d->op->sv), create_type(d, base_type));
      }
      return;
    }
    if (typeid(*ast) == typeid(ASTFuncDef)) {
      std::shared_ptr<ASTFuncDef> n = std::dynamic_pointer_cast<ASTFuncDef>(ast);
      std::vector<std::shared_ptr<EvalType>> type_args;
      for (
        std::shared_ptr<ASTSimpleDeclaration> d: n->declaration->declarator->args
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
    if (typeid(*ast) == typeid(ASTDeclaration)) {
      std::shared_ptr<ASTDeclaration> n = std::dynamic_pointer_cast<ASTDeclaration>(ast);
      std::shared_ptr<EvalType> base_type = create_base_type(n->declaration_spec);
      for (std::shared_ptr<ASTDeclarator> d: n->declarators) {
        se->add_local_var(std::string(d->op->sv), create_type(d, base_type));
      }
      return;
    }
    if (typeid(*ast) == typeid(ASTCompoundStmt)) {
      std::shared_ptr<ASTCompoundStmt> n = std::dynamic_pointer_cast<ASTCompoundStmt>(ast);
      se->start_scope();
      for (std::shared_ptr<AST> stmt: n->items) {
        generator_sub(stmt, se, code);
      }
      se->end_scope();
    }
    if (typeid(*ast) == typeid(ASTExprStmt)) {
      std::shared_ptr<ASTExprStmt> n = std::dynamic_pointer_cast<ASTExprStmt>(ast);
      generator_sub(n->expr, se, code);
      code += "pop r10\n"; // pop the value that need not be evaluate
    }
    if (typeid(*ast) == typeid(ASTReturnStmt)) {
      std::shared_ptr<ASTReturnStmt> n = std::dynamic_pointer_cast<ASTReturnStmt>(ast);
      generator_sub(n->expr, se, code);
      code += "pop rax\n"; // set return value
      code += "mov rsp, rbp\n";
      code += "pop rbp\n";
      code += "ret\n";
    }
    if (typeid(*ast) == typeid(ASTBreakStmt)) {
      std::string label = se->get_loop_info().label_break;
      if (!label.length()) {
        // TODO error
      }
      code += "jmp L" + label;
      return;
    }
    if (typeid(*ast) == typeid(ASTContinueStmt)) {
      std::string label = se->get_loop_info().label_continue;
      if (!label.length()) {
        // TODO error
      }
      code += "jmp L" + label;
      return;
    }
    if (typeid(*ast) == typeid(ASTAssignExpr)) {}
    if (typeid(*ast) == typeid(ASTLogicalOrExpr)) {}
    if (typeid(*ast) == typeid(ASTLogicalAndExpr)) {}
    if (typeid(*ast) == typeid(ASTBitwiseOrExpr)) {}
    if (typeid(*ast) == typeid(ASTBitwiseXorExpr)) {}
    if (typeid(*ast) == typeid(ASTBitwiseAndExpr)) {}
    if (typeid(*ast) == typeid(ASTEqualityExpr)) {}
    if (typeid(*ast) == typeid(ASTRelationalExpr)) {}
    if (typeid(*ast) == typeid(ASTShiftExpr)) {}
    if (typeid(*ast) == typeid(ASTAdditiveExpr)) {}
    if (typeid(*ast) == typeid(ASTMultiplicativeExpr)) {}
    if (typeid(*ast) == typeid(ASTFuncCallExpr)) {}
    if (typeid(*ast) == typeid(ASTPrimaryExpr)) {}
    if (typeid(*ast) == typeid(ASTSimpleExpr)) {}
  }

  std::string generator(std::shared_ptr<AST> ast) {
    std::string ret;
    std::shared_ptr<SymbolEntrys> symbol_entrys = std::make_shared<SymbolEntrys>();
    generator_sub(ast, symbol_entrys, ret);
    return ret;
  }
}