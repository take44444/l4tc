#include "./generator.hpp"

namespace generator {
  const std::string param_reg_names[6] = {"rdi", "rsi", "rdx", "rcx", "r8",  "r9"};
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

  void generator_sub(std::shared_ptr<AST> ast, std::shared_ptr<Context> ctx, std::string &code) {
    if (typeid(*ast) == typeid(ASTTranslationUnit)) {
      std::shared_ptr<ASTTranslationUnit> n = std::dynamic_pointer_cast<ASTTranslationUnit>(ast);
      code += ".intel_syntax noprefix\n"; // use intel syntax
      code += ".text\n";
      for (std::shared_ptr<AST> d: n->external_declarations) {
        generator_sub(d, ctx, code);
      }
      return;
    }
    // declaration-spec simple-declarators
    if (typeid(*ast) == typeid(ASTExternalDeclaration)) {
      std::shared_ptr<ASTExternalDeclaration> n = std::dynamic_pointer_cast<ASTExternalDeclaration>(ast);
      std::shared_ptr<EvalType> base_type = create_base_type(n->declaration_spec);
      for (std::shared_ptr<ASTDeclarator> d: n->declarators) {
        ctx->add_global_var(std::string(d->op->sv), create_type(d, base_type));
      }
      return;
    }
    if (typeid(*ast) == typeid(ASTFuncDef)) {
      std::shared_ptr<ASTFuncDef> n = std::dynamic_pointer_cast<ASTFuncDef>(ast);
      std::vector<std::shared_ptr<EvalType>> type_args;
      std::vector<std::string> name_args;
      std::string func_name = std::string(n->declaration->declarator->declarator->op->sv);
      for (
        std::shared_ptr<ASTSimpleDeclaration> d: n->declaration->declarator->args
      ) {
        type_args.push_back(create_type(d->declarator, create_base_type(d->type_spec)));
        name_args.push_back(std::string(d->declarator->op->sv));
      }
      ctx->add_func(
        func_name,
        create_type(
          n->declaration->declarator->declarator,
          create_base_type(n->declaration->type_spec)
        ),
        type_args
      );
      code += ".global " + func_name + "\n";
      code += func_name + ":\n";
      assert(ctx->rsp == 0); // here is global
      ctx->start_scope(); // remember rsp value
      code += "push rbp\n";
      code += "mov rbp rsp\n";
      ctx->rsp = 0; // rsp == rbp
      if (type_args.size() > 6) {
        // TODO: the maximum number of arguments of function is 6
      }
      // sub number of params * 8 from rsp
      code += "sub rsp " + std::to_string((int)type_args.size() * 8) + "\n";
      for (int i=0; i < (int)type_args.size(); i++) {
        // all size of vars are 8 byte (64bit) in this lupc
        ctx->rsp -= 8;
        // add vars in function arguments to local vars
        ctx->add_local_var(name_args[i], type_args[i]);
        code += "mov [rbp - " + std::to_string(-ctx->rsp) + "] " + param_reg_names[i] + "\n";
      }
      generator_sub(n->body, ctx, code);
      // pop arguments from stack
      code += "add rsp " + std::to_string((int)type_args.size() * 8) + "\n";
      ctx->rsp += (int)type_args.size() * 8;
      ctx->end_scope(); // check rsp
      code += "mov rsp, rbp\n";
      code += "pop rbp\n";
      code += "ret\n"; // default return
    }
    if (typeid(*ast) == typeid(ASTDeclaration)) {
      std::shared_ptr<ASTDeclaration> n = std::dynamic_pointer_cast<ASTDeclaration>(ast);
      std::shared_ptr<EvalType> base_type = create_base_type(n->declaration_spec);
      // sub number of declarators * 8 from rsp
      code += "sub rsp " + std::to_string((int)n->declarators.size() * 8) + "\n";
      for (std::shared_ptr<ASTDeclarator> d: n->declarators) {
        // all size of vars are 8 byte (64bit) in this lupc
        ctx->rsp -= 8;
        // add vars in function arguments to local vars
        ctx->add_local_var(std::string(d->op->sv), create_type(d, base_type));
      }
      return;
    }
    if (typeid(*ast) == typeid(ASTCompoundStmt)) {
      std::shared_ptr<ASTCompoundStmt> n = std::dynamic_pointer_cast<ASTCompoundStmt>(ast);
      int saved_rsp1 = ctx->rsp;
      for (std::shared_ptr<AST> stmt: n->items) {
        if (typeid(*stmt) == typeid(ASTDeclaration)) generator_sub(stmt, ctx, code);
      }
      int saved_rsp2 = ctx->rsp;
      for (std::shared_ptr<AST> stmt: n->items) {
        if (typeid(*stmt) == typeid(ASTDeclaration)) continue;
        if (typeid(*stmt) == typeid(ASTCompoundStmt)) ctx->start_scope();
        generator_sub(stmt, ctx, code);
        if (typeid(*stmt) == typeid(ASTCompoundStmt)) ctx->end_scope();
      }
      assert(ctx->rsp == saved_rsp2);
      code += "add rsp " + std::to_string(saved_rsp1 - saved_rsp2) + "\n";
      ctx->rsp += saved_rsp1 - saved_rsp2;
      return;
    }
    if (typeid(*ast) == typeid(ASTExprStmt)) {
      std::shared_ptr<ASTExprStmt> n = std::dynamic_pointer_cast<ASTExprStmt>(ast);
      generator_sub(n->expr, ctx, code);
      code += "pop r11\n"; // pop the value that need not be evaluate
      return;
    }
    if (typeid(*ast) == typeid(ASTReturnStmt)) {
      std::shared_ptr<ASTReturnStmt> n = std::dynamic_pointer_cast<ASTReturnStmt>(ast);
      generator_sub(n->expr, ctx, code);
      code += "pop rax\n"; // set return value
      code += "add rsp " + std::to_string(se->func_saved_rsp() - se->rsp) + "\n";
      code += "mov rsp, rbp\n";
      code += "pop rbp\n";
      code += "ret\n";
    }
    if (typeid(*ast) == typeid(ASTBreakStmt)) {
      std::string label = ctx->get_loop_info().label_break;
      if (!label.length()) {
        // TODO error
      }
      code += "jmp L" + label;
      return;
    }
    if (typeid(*ast) == typeid(ASTContinueStmt)) {
      std::string label = ctx->get_loop_info().label_continue;
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
    std::shared_ptr<Context> context = std::make_shared<Context>();
    generator_sub(ast, context, ret);
    return ret;
  }
}