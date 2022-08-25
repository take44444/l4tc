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
    // case KwVoid:
    //   return std::make_shared<TypeVoid>();
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
      code += "mov rbp, rsp\n";
      ctx->rsp = 0; // rsp == rbp
      if (type_args.size() > 6) {
        // TODO: the maximum number of arguments of function is 6
      }
      // sub number of params * 8 from rsp
      code += "sub rsp, " + std::to_string((int)type_args.size() * 8) + "\n";
      for (int i=0; i < (int)type_args.size(); i++) {
        // all size of vars are 8 byte (64bit) in this lupc
        ctx->rsp -= 8;
        // add vars in function arguments to local vars
        ctx->add_local_var(name_args[i], type_args[i]);
        code += "mov [rbp - " + std::to_string(-ctx->rsp) + "], " + param_reg_names[i] + "\n";
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
      code += "sub rsp, " + std::to_string((int)n->declarators.size() * 8) + "\n";
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
      code += "add rsp, " + std::to_string(saved_rsp1 - saved_rsp2) + "\n";
      ctx->rsp += saved_rsp1 - saved_rsp2;
      return;
    }
    if (typeid(*ast) == typeid(ASTExprStmt)) {
      std::shared_ptr<ASTExprStmt> n = std::dynamic_pointer_cast<ASTExprStmt>(ast);
      generator_sub(n->expr, ctx, code);
      ctx->rsp += 8;
      code += "pop r10\n"; // pop the value that need not be evaluate
      return;
    }
    if (typeid(*ast) == typeid(ASTReturnStmt)) {
      std::shared_ptr<ASTReturnStmt> n = std::dynamic_pointer_cast<ASTReturnStmt>(ast);
      generator_sub(n->expr, ctx, code);
      ctx->rsp += 8;
      code += "pop rax\n"; // set return value
      code += "add rsp, " + std::to_string(ctx->func_saved_rsp() - ctx->rsp) + "\n";
      code += "mov rsp, rbp\n";
      code += "pop rbp\n";
      code += "ret\n";
    }
    // if (typeid(*ast) == typeid(ASTBreakStmt)) {
    //   std::string label = ctx->get_loop_info().label_break;
    //   if (!label.length()) {
    //     // TODO error
    //   }
    //   code += "jmp L" + label;
    //   return;
    // }
    // if (typeid(*ast) == typeid(ASTContinueStmt)) {
    //   std::string label = ctx->get_loop_info().label_continue;
    //   if (!label.length()) {
    //     // TODO error
    //   }
    //   code += "jmp L" + label;
    //   return;
    // }
    if (typeid(*ast) == typeid(ASTAssignExpr)) {
      std::shared_ptr<ASTAssignExpr> n = std::dynamic_pointer_cast<ASTAssignExpr>(ast);
      generator_sub(n->right, ctx, code);
      generator_sub(n->left, ctx, code);
      if (!n->left->is_assignable) {
        // TODO error
      }
      if (typeid(n->left->eval_type) != typeid(n->right->eval_type)) {
        // TODO error
      }
      code += "pop r11\n";
      code += "pop r10\n";
      if (n->right->is_assignable) code += "mov r11 [r11]\n";
      code += "mov [r10], r11\n";
      code += "mov r11, [r10]\n";
      code += "push r11\n";
      ctx->rsp += 8;
      n->eval_type = n->left->eval_type;
      n->is_assignable = false;
    }
    // if (typeid(*ast) == typeid(ASTLogicalOrExpr)) {}
    // if (typeid(*ast) == typeid(ASTLogicalAndExpr)) {}
    // if (typeid(*ast) == typeid(ASTBitwiseOrExpr)) {}
    // if (typeid(*ast) == typeid(ASTBitwiseXorExpr)) {}
    // if (typeid(*ast) == typeid(ASTBitwiseAndExpr)) {}
    // if (typeid(*ast) == typeid(ASTEqualityExpr)) {}
    // if (typeid(*ast) == typeid(ASTRelationalExpr)) {}
    // if (typeid(*ast) == typeid(ASTShiftExpr)) {}
    if (typeid(*ast) == typeid(ASTAdditiveExpr)) {
      std::shared_ptr<ASTAdditiveExpr> n = std::dynamic_pointer_cast<ASTAdditiveExpr>(ast);
      generator_sub(n->left, ctx, code);
      generator_sub(n->right, ctx, code);
      if (typeid(n->left->eval_type) != typeid(TypeNum)) {
        // TODO error
      }
      if (typeid(n->right->eval_type) != typeid(TypeNum)) {
        // TODO error
      }
      code += "pop r11\n";
      code += "pop r10\n";
      if (n->left->is_assignable) code += "mov r10 [r10]\n";
      if (n->right->is_assignable) code += "mov r11 [r11]\n";
      code += "add r10, r11\n";
      code += "push r10\n";
      ctx->rsp += 8;
      n->eval_type = n->left->eval_type;
      n->is_assignable = false;
    }
    if (typeid(*ast) == typeid(ASTMultiplicativeExpr)) {
      std::shared_ptr<ASTMultiplicativeExpr> n = std::dynamic_pointer_cast<ASTMultiplicativeExpr>(ast);
      generator_sub(n->left, ctx, code);
      generator_sub(n->right, ctx, code);
      if (typeid(n->left->eval_type) != typeid(TypeNum)) {
        // TODO error
      }
      if (typeid(n->right->eval_type) != typeid(TypeNum)) {
        // TODO error
      }
      code += "pop r11\n";
      code += "pop r10\n";
      if (n->left->is_assignable) code += "mov r10 [r10]\n";
      if (n->right->is_assignable) code += "mov r11 [r11]\n";
      code += "imul r10, r11\n";
      code += "push r10\n";
      ctx->rsp += 8;
      n->eval_type = n->left->eval_type;
      n->is_assignable = false;
    }
    if (typeid(*ast) == typeid(ASTFuncCallExpr)) {
      std::shared_ptr<ASTFuncCallExpr> n = std::dynamic_pointer_cast<ASTFuncCallExpr>(ast);
      generator_sub(n->primary, ctx, code);
      // ctx->get_func(n->primary->)
    }
    if (typeid(*ast) == typeid(ASTPrimaryExpr)) {
      std::shared_ptr<ASTPrimaryExpr> n = std::dynamic_pointer_cast<ASTPrimaryExpr>(ast);
      generator_sub(n->expr, ctx, code);
      n->eval_type = n->expr->eval_type;
      n->is_assignable = n->expr->is_assignable;
    }
    if (typeid(*ast) == typeid(ASTSimpleExpr)) {
      std::shared_ptr<ASTSimpleExpr> n = std::dynamic_pointer_cast<ASTSimpleExpr>(ast);
      if (n->op->type == Ident) {
        LocalVarInfo lvi = ctx->get_local_var_info(n->op->sv);
        n->eval_type = lvi.type;
        n->is_assignable = true;
        code += "lea r10, [rbp - " + std::to_string(lvi.offset) + "]\n";
        code += "push r10\n";
        ctx->rsp -= 8;
      } else if (n->op->type == NumberConstant) {
        n->eval_type = std::make_shared<TypeNum>();
        n->is_assignable = false;
        code += "mov r10, " + std::string(n->op->sv) + "\n";
        code += "push r10\n";
        ctx->rsp -= 8;
      }
      return;
    }
  }

  std::string generator(std::shared_ptr<AST> ast) {
    std::string ret;
    std::shared_ptr<Context> context = std::make_shared<Context>();
    generator_sub(ast, context, ret);
    return ret;
  }
}