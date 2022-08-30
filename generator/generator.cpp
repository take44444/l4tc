#include "./generator.hpp"

namespace generator {
  static int label_number = 0;
  const std::string param_reg_names[6] = {"rdi", "rsi", "rdx", "rcx", "r8",  "r9"};

  bool type_eq(std::shared_ptr<EvalType> x, std::shared_ptr<EvalType> y) {
    if (typeid(*x) == typeid(TypeAny) || typeid(*y) == typeid(TypeAny)) return true;
    if (typeid(*x) != typeid(*y)) return false;
    if (typeid(*x) == typeid(TypeArray)) {
      assert(typeid(*y) == typeid(TypeArray));
      std::shared_ptr<TypeArray> xx = std::dynamic_pointer_cast<TypeArray>(x);
      std::shared_ptr<TypeArray> yy = std::dynamic_pointer_cast<TypeArray>(y);
      return xx->size == yy->size && type_eq(xx->array_of, yy->array_of);
    }
    // TODO recursive check for type of pointer, function
    return true;
  }

  bool is_aligned(int rsp) {
    return !(rsp & 0xF);
  }

  std::shared_ptr<EvalType> create_base_type(std::shared_ptr<ASTTypeSpec> n) {
    // TODO static, const
    switch (n->op->type)
    {
    case KwNum:
      return std::make_shared<TypeNum>();
    default:
      break;
    }
    assert(false);
    return nullptr;
  }

  std::shared_ptr<EvalType> create_type(std::shared_ptr<ASTDeclarator>, std::shared_ptr<EvalType> base_type) {
    // TODO: pointer
    return base_type;
  }

  std::shared_ptr<TypeFunc> create_func_type(std::shared_ptr<ASTFuncDeclaration> fd) {
    std::vector<std::shared_ptr<EvalType>> type_args;
    for (std::shared_ptr<ASTSimpleDeclaration> d: fd->declarator->args) {
      type_args.push_back(
        create_type(d->declarator, create_base_type(d->type_spec))
      );
    }
    return std::make_shared<TypeFunc>(
      type_args,
      create_type(
        fd->declarator->declarator,
        create_base_type(fd->type_spec)
      )
    );
  }

  void generate_text_section(std::shared_ptr<AST> ast, std::shared_ptr<Context> ctx, std::string &code) {
    if (typeid(*ast) == typeid(ASTTranslationUnit)) {
      std::shared_ptr<ASTTranslationUnit> n = std::dynamic_pointer_cast<ASTTranslationUnit>(ast);
      code += ".intel_syntax noprefix\n"; // use intel syntax
      code += ".text\n"; // text section
      for (std::shared_ptr<AST> d: n->external_declarations) {
        generate_text_section(d, ctx, code);
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
      std::shared_ptr<ASTFuncDeclaration> fd = n->declaration;
      std::string func_name = std::string(fd->declarator->declarator->op->sv);
      std::shared_ptr<TypeFunc> tf = create_func_type(fd);
      std::vector<std::string> name_args;
      if (fd->declarator->args.size() > 6) {
        // TODO: the maximum number of arguments of function is 6 in lup
        assert(false);
      }
      for (std::shared_ptr<ASTSimpleDeclaration> d: fd->declarator->args) {
        name_args.push_back(std::string(d->declarator->op->sv));
      }
      ctx->add_global_var(func_name, tf);
      code += ".global " + func_name + "\n";
      code += func_name + ":\n";
      ctx->rsp = 0; // now rsp == rbp
      ctx->start_scope();
      code += "push rbp\n";
      code += "mov rbp, rsp\n";
      // push arguments
      code += "sub rsp, " + std::to_string(
        ((int)name_args.size() +
        ((int)name_args.size()&1)) * 8
      ) + "\n";
      for (int i=0; i < (int)name_args.size(); i++) {
        // all size of vars are 8 byte (64bit) in this lupc
        ctx->rsp -= 8;
        // add vars in function arguments to local vars
        ctx->add_local_var(name_args[i], tf->type_args[i]);
        code += "mov [rbp - " + std::to_string(-ctx->rsp) + "], " + param_reg_names[i] + "\n";
      }
      ctx->rsp -= ((int)name_args.size()&1) * 8;
      generate_text_section(n->body, ctx, code);
      ctx->end_scope();
      code += "mov rsp, rbp\n";
      code += "pop rbp\n";
      code += "ret\n"; // default return
    }
    if (typeid(*ast) == typeid(ASTDeclaration)) {
      std::shared_ptr<ASTDeclaration> n = std::dynamic_pointer_cast<ASTDeclaration>(ast);
      std::shared_ptr<EvalType> base_type = create_base_type(n->declaration_spec);
      code += "sub rsp, " + std::to_string(
        ((int)n->declarators.size() +
        ((int)n->declarators.size()&1)) * 8
      ) + "\n";
      for (std::shared_ptr<ASTDeclarator> d: n->declarators) {
        // all size of vars are 8 byte (64bit) in this lupc
        ctx->rsp -= 8;
        // add vars in function arguments to local vars
        ctx->add_local_var(std::string(d->op->sv), create_type(d, base_type));
      }
      ctx->rsp -= ((int)n->declarators.size()&1) * 8;
      return;
    }
    if (typeid(*ast) == typeid(ASTIfStmt)) {
      std::shared_ptr<ASTIfStmt> n = std::dynamic_pointer_cast<ASTIfStmt>(ast);
      int false_label = label_number++;
      int end_label = label_number++;
      generate_text_section(n->cond, ctx, code);
      ctx->rsp += 8;
      code += "pop r10\n";
      if (n->cond->is_assignable) code += "mov r10, [r10]\n";
      code += "cmp r10, 0\n";
      code += "setnz r10b\n";
      code += "jz L" + std::to_string(false_label) + "\n";
      generate_text_section(n->true_stmt, ctx, code);
      code += "jmp L" + std::to_string(end_label) + "\n";
      code += "L" + std::to_string(false_label) + ":\n";
      if (n->false_stmt) generate_text_section(n->false_stmt, ctx, code);
      code += "L" + std::to_string(end_label) + ":\n";
    }
    if (typeid(*ast) == typeid(ASTElseStmt)) {
      std::shared_ptr<ASTElseStmt> n = std::dynamic_pointer_cast<ASTElseStmt>(ast);
      int false_label = label_number++;
      int end_label = label_number++;
      if (n->cond) {
        generate_text_section(n->cond, ctx, code);
        ctx->rsp += 8;
        code += "pop r10\n";
        if (n->cond->is_assignable) code += "mov r10, [r10]\n";
        code += "cmp r10, 0\n";
        code += "setnz r10b\n";
        code += "jz L" + std::to_string(false_label) + "\n";
      }
      generate_text_section(n->true_stmt, ctx, code);
      code += "jmp L" + std::to_string(end_label) + "\n";
      code += "L" + std::to_string(false_label) + ":\n";
      if (n->false_stmt) generate_text_section(n->false_stmt, ctx, code);
      code += "L" + std::to_string(end_label) + ":\n";
    }
    if (typeid(*ast) == typeid(ASTCompoundStmt)) {
      std::shared_ptr<ASTCompoundStmt> n = std::dynamic_pointer_cast<ASTCompoundStmt>(ast);
      ctx->start_scope();
      int saved_rsp = ctx->rsp;
      for (std::shared_ptr<AST> stmt: n->items) {
        if (typeid(*stmt) == typeid(ASTDeclaration)) generate_text_section(stmt, ctx, code);
      }
      for (std::shared_ptr<AST> stmt: n->items) {
        if (typeid(*stmt) == typeid(ASTDeclaration)) continue;
        generate_text_section(stmt, ctx, code);
      }
      ctx->rsp = saved_rsp;
      code += "mov rsp, " + std::to_string(saved_rsp) + "\n";
      ctx->end_scope();
      return;
    }
    if (typeid(*ast) == typeid(ASTExprStmt)) {
      std::shared_ptr<ASTExprStmt> n = std::dynamic_pointer_cast<ASTExprStmt>(ast);
      generate_text_section(n->expr, ctx, code);
      ctx->rsp += 8;
      code += "pop r10\n"; // pop the value that need not be evaluate
      return;
    }
    if (typeid(*ast) == typeid(ASTReturnStmt)) {
      std::shared_ptr<ASTReturnStmt> n = std::dynamic_pointer_cast<ASTReturnStmt>(ast);
      std::shared_ptr<ASTExpr> expr = std::dynamic_pointer_cast<ASTExpr>(n->expr);
      generate_text_section(expr, ctx, code);
      ctx->rsp += 8;
      code += "pop rax\n"; // set return value
      if (expr->is_assignable) code += "mov rax, [rax]\n";
      code += "mov rsp, rbp\n";
      code += "pop rbp\n";
      code += "ret\n";
      return;
    }
    // if (typeid(*ast) == typeid(ASTBreakStmt)) {
    //   std::string label = ctx->get_loop().label_break;
    //   // TODO rsp
    //   if (!label.length()) {
    //     // TODO error
    //     assert(false);
    //   }
    //   code += "jmp L" + label;
    //   return;
    // }
    // if (typeid(*ast) == typeid(ASTContinueStmt)) {
    //   std::string label = ctx->get_loop().label_continue;
    //   // TODO rsp
    //   if (!label.length()) {
    //     // TODO error
    //     assert(false);
    //   }
    //   code += "jmp L" + label;
    //   return;
    // }
    if (typeid(*ast) == typeid(ASTAssignExpr)) {
      std::shared_ptr<ASTAssignExpr> n = std::dynamic_pointer_cast<ASTAssignExpr>(ast);
      generate_text_section(n->right, ctx, code);
      generate_text_section(n->left, ctx, code);
      if (!n->left->is_assignable) {
        // TODO error
        assert(false);
      }
      if (!type_eq(n->left->eval_type,n->right->eval_type)) {
        // TODO error
        assert(false);
      }
      code += "pop r10\n";
      code += "pop r11\n";
      if (n->right->is_assignable) code += "mov r11 [r11]\n";
      code += "mov [r10], r11\n";
      // code += "mov r11, [r10]\n";
      code += "push r11\n";
      ctx->rsp += 8; // 2 pop 1 push
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
      generate_text_section(n->left, ctx, code);
      generate_text_section(n->right, ctx, code);
      if (typeid(*n->left->eval_type) != typeid(TypeNum)) {
        // TODO error
        assert(false);
      }
      if (typeid(*n->right->eval_type) != typeid(TypeNum)) {
        // TODO error
        assert(false);
      }
      code += "pop r11\n";
      code += "pop r10\n";
      if (n->left->is_assignable) code += "mov r10, [r10]\n";
      if (n->right->is_assignable) code += "mov r11, [r11]\n";
      if (n->op->sv == "+") code += "add r10, r11\n";
      else code += "sub r10, r11\n";
      code += "push r10\n";
      ctx->rsp += 8; // 2 pop and 1 push
      n->eval_type = n->left->eval_type;
      n->is_assignable = false;
    }
    if (typeid(*ast) == typeid(ASTMultiplicativeExpr)) {
      std::shared_ptr<ASTMultiplicativeExpr> n = std::dynamic_pointer_cast<ASTMultiplicativeExpr>(ast);
      generate_text_section(n->left, ctx, code);
      generate_text_section(n->right, ctx, code);
      if (typeid(*n->left->eval_type) != typeid(TypeNum)) {
        // TODO error
        assert(false);
      }
      if (typeid(*n->right->eval_type) != typeid(TypeNum)) {
        // TODO error
        assert(false);
      }
      code += "pop r11\n";
      code += "pop r10\n";
      if (n->left->is_assignable) code += "mov r10, [r10]\n";
      if (n->right->is_assignable) code += "mov r11, [r11]\n";
      if (n->op->sv == "*") code += "imul r10, r11\n";
      else code += "idiv r10, r11\n";
      code += "push r10\n";
      ctx->rsp += 8; // 2 pop and 1 push
      n->eval_type = n->left->eval_type;
      n->is_assignable = false;
    }
    if (typeid(*ast) == typeid(ASTFuncCallExpr)) {
      std::shared_ptr<ASTFuncCallExpr> n = std::dynamic_pointer_cast<ASTFuncCallExpr>(ast);
      generate_text_section(n->primary, ctx, code);
      if (typeid(*(n->primary->eval_type)) != typeid(TypeFunc)) {
        // TODO error
        assert(false);
      }
      if (n->args.size() > 6) {
        // TODO: the maximum number of arguments of function is 6 in lup
        assert(false);
      }
      std::shared_ptr<TypeFunc> tf = std::dynamic_pointer_cast<TypeFunc>(n->primary->eval_type);
      for (int i=0; i < (int)n->args.size(); i++) generate_text_section(n->args[i], ctx, code);
      if (typeid(*tf->ret_type) != typeid(TypeAny)) {
        if (n->args.size() != tf->type_args.size()) {
          // TODO error
          assert(false);
        }
        for (int i=0; i < (int)n->args.size(); i++) {
          if (!type_eq(n->args[i]->eval_type, tf->type_args[i])) {
            // TODO error
            assert(false);
          }
        }
      }
      for (int i=(int)n->args.size()-1; i >= 0; i--) {
        code += "pop " + param_reg_names[i] + "\n";
        if (n->args[i]->is_assignable) {
          code += "mov " + param_reg_names[i] + ", [" + param_reg_names[i] +"]\n";
        }
      }
      code += "pop rax\n";
      ctx->rsp += ((int)n->args.size() + 1) * 8;
      // rsp needs to be aligned when call
      if (!is_aligned(ctx->rsp)) code += "sub rsp, 8\n";
      code += "call rax\n";
      if (!is_aligned(ctx->rsp)) code += "add rsp, 8\n";
      ctx->rsp -= 8;
      code += "push rax\n";
      n->eval_type = tf->ret_type;
      n->is_assignable = false;
      return;
    }
    if (typeid(*ast) == typeid(ASTPrimaryExpr)) {
      std::shared_ptr<ASTPrimaryExpr> n = std::dynamic_pointer_cast<ASTPrimaryExpr>(ast);
      generate_text_section(n->expr, ctx, code);
      n->eval_type = n->expr->eval_type;
      n->is_assignable = n->expr->is_assignable;
      return;
    }
    if (typeid(*ast) == typeid(ASTSimpleExpr)) {
      std::shared_ptr<ASTSimpleExpr> n = std::dynamic_pointer_cast<ASTSimpleExpr>(ast);
      if (n->op->type == Ident) {
        std::shared_ptr<LocalVar> lvi = ctx->get_local_var(n->op->sv);
        if (lvi) {
          code += "lea r10, [rbp - " + std::to_string(lvi->offset) + "]\n";
          code += "push r10\n";
          ctx->rsp -= 8;
          n->eval_type = lvi->type;
          n->is_assignable = true;
          return;
        }
        std::shared_ptr<GlobalVar> gvi = ctx->get_global_var(n->op->sv);
        if (gvi) {
          code += ".global " + gvi->name + "\n";
          code += "mov r10, [rip + " + gvi->name + "@GOTPCREL]\n";
          code += "push r10\n";
          ctx->rsp -= 8;
          n->eval_type = gvi->type;
          n->is_assignable = typeid(*n->eval_type) != typeid(TypeFunc);
          return;
        }
        // TODO get extern
        code += ".global " + std::string(n->op->sv) + "\n";
        code += "mov r10, [rip + " + std::string(n->op->sv) + "@GOTPCREL]\n";
        code += "push r10\n";
        ctx->rsp -= 8;
        n->eval_type = std::make_shared<TypeFunc>(std::make_shared<TypeAny>());
        n->is_assignable = false;
        return;
      } else if (n->op->type == NumberConstant) {
        code += "mov r10, " + std::string(n->op->sv) + "\n";
        code += "push r10\n";
        ctx->rsp -= 8;
        n->eval_type = std::make_shared<TypeNum>();
        n->is_assignable = false;
        return;
      } else if (n->op->type == StringLiteral) {
        std::string label = "L" + std::to_string(label_number++);
        code += "lea r10, [rip + " + label + "]\n";
        code += "push r10\n";
        ctx->rsp -= 8;
        ctx->add_str(label, n->op->sv);
        n->eval_type = std::make_shared<TypeArray>(
          std::make_shared<TypeChar>(), n->op->sv.length() - 2
        );
        n->is_assignable = false;
        return;
      }
      assert(false);
    }
  }

  void generate_data_section(std::shared_ptr<Context> ctx, std::string &code) {
    code += ".data\n";
    for (std::pair<std::string, std::string> &d: ctx->strs) {
      code += d.first + ": .asciz " + d.second + "\n";
    }
  }

  std::string generate(std::shared_ptr<AST> ast) {
    std::string ret;
    std::shared_ptr<Context> context = std::make_shared<Context>();
    generate_text_section(ast, context, ret);
    generate_data_section(context, ret);
    return ret;
  }
}