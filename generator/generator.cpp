#include "./generator.hpp"

namespace generator {

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
      std::shared_ptr<EvalType> type = create_type(n->declaration_spec);
      for (std::shared_ptr<ASTDeclarator> d: n->declarators) {
        ctx->add_global_var(std::string(d->op->sv), type);
      }
      return;
    }
    if (typeid(*ast) == typeid(ASTFuncDef)) {
      std::shared_ptr<ASTFuncDef> n = std::dynamic_pointer_cast<ASTFuncDef>(ast);
      std::shared_ptr<ASTFuncDeclaration> fd = n->declaration;
      std::string func_name = std::string(fd->declarator->declarator->op->sv);
      std::shared_ptr<TypeFunc> tf = create_func_type(fd);
      if (fd->declarator->args.size() > 6) {
        assert(false);
      }
      ctx->add_func(func_name, tf);
      code += ".global " + func_name + "\n";
      code += func_name + ":\n";
      code += "push rbp\n";
      code += "mov rbp, rsp\n";
      ctx->rsp = 0; // now rsp == rbp
      ctx->start_scope();
      for (int i=0; i < (int)tf->type_args.size(); i++) {
        push_args(i, tf->type_args[i], ctx, code);
        ctx->add_local_var(
          std::string(fd->declarator->args[i]->declarator->op->sv),
          tf->type_args[i]
        );
      }
      generate_text_section(n->body, ctx, code);
      ctx->end_scope();
      ret(code);
      return;
    }
    if (typeid(*ast) == typeid(ASTDeclaration)) {
      std::shared_ptr<ASTDeclaration> n = std::dynamic_pointer_cast<ASTDeclaration>(ast);
      std::shared_ptr<EvalType> type = create_type(n->declaration_spec);
      for (std::shared_ptr<ASTDeclarator> d: n->declarators) {
        code += "sub rsp, " + std::to_string(align_8(type->size)) + "\n";
        ctx->rsp -= align_8(type->size);
        ctx->add_local_var(std::string(d->op->sv), type);
      }
      return;
    }
    if (typeid(*ast) == typeid(ASTIfStmt)) {
      std::shared_ptr<ASTIfStmt> n = std::dynamic_pointer_cast<ASTIfStmt>(ast);
      std::string false_label = create_label();
      std::string end_label = create_label();
      generate_text_section(n->cond, ctx, code);
      if (typeid(*n->cond->eval_type) == typeid(TypeArray)) {
        assert(false);
      }
      ctx->rsp += 8;
      code += "pop r10\n";
      eval(n->cond, "r10", code);
      code += "cmp r10, 0\n";
      code += "setnz r10b\n";
      code += "jz " + false_label + "\n";
      generate_text_section(n->true_stmt, ctx, code);
      code += "jmp " + end_label + "\n";
      code += false_label + ":\n";
      if (n->false_stmt) generate_text_section(n->false_stmt, ctx, code);
      code += end_label + ":\n";
    }
    if (typeid(*ast) == typeid(ASTElseStmt)) {
      std::shared_ptr<ASTElseStmt> n = std::dynamic_pointer_cast<ASTElseStmt>(ast);
      std::string false_label = create_label();
      std::string end_label = create_label();
      if (n->cond) {
        generate_text_section(n->cond, ctx, code);
        if (typeid(*n->cond->eval_type) == typeid(TypeArray)) {
          assert(false);
        }
        ctx->rsp += 8;
        code += "pop r10\n";
        eval(n->cond, "r10", code);
        code += "cmp r10, 0\n";
        code += "setnz r10b\n";
        code += "jz " + false_label + "\n";
      }
      generate_text_section(n->true_stmt, ctx, code);
      code += "jmp " + end_label + "\n";
      code += false_label + ":\n";
      if (n->false_stmt) generate_text_section(n->false_stmt, ctx, code);
      code += end_label + ":\n";
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
      code += "lea rsp, [rbp - " + std::to_string(-saved_rsp) + "]\n";
      ctx->end_scope();
      return;
    }
    if (typeid(*ast) == typeid(ASTExprStmt)) {
      std::shared_ptr<ASTExprStmt> n = std::dynamic_pointer_cast<ASTExprStmt>(ast);
      generate_text_section(n->expr, ctx, code);
      pop("r10", ctx, code);
      return;
    }
    if (typeid(*ast) == typeid(ASTReturnStmt)) {
      std::shared_ptr<ASTReturnStmt> n = std::dynamic_pointer_cast<ASTReturnStmt>(ast);
      std::shared_ptr<ASTExpr> expr = std::dynamic_pointer_cast<ASTExpr>(n->expr);
      generate_text_section(expr, ctx, code);
      pop("rax", ctx, code);
      eval(expr, "rax", code);
      ret(code);
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
      if (!try_assign(n->left, n->right, ctx, code)) {
        assert(false);
      }

      n->eval_type = n->left->eval_type;
      n->is_assignable = false;
      return;
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
      pop("r11", ctx, code);
      pop("r10", ctx, code);
      eval(n->left, "r10", code);
      eval(n->right, "r11", code);
      if (n->op->sv == "+") code += "add r10, r11\n";
      else code += "sub r10, r11\n";
      push("r10", ctx, code);
      n->eval_type = n->left->eval_type;
      n->is_assignable = false;
      return;
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
      pop("r11", ctx, code);
      pop("r10", ctx, code);
      eval(n->left, "r10", code);
      eval(n->right, "r11", code);
      if (n->op->sv == "*") code += "imul r10, r11\n";
      else code += "idiv r10, r11\n";
      push("r10", ctx, code);
      n->eval_type = n->left->eval_type;
      n->is_assignable = false;
      return;
    }
    if (typeid(*ast) == typeid(ASTUnaryExpr)) {
      std::shared_ptr<ASTUnaryExpr> n = std::dynamic_pointer_cast<ASTUnaryExpr>(ast);
      generate_text_section(n->expr, ctx, code);
      if (n->op->sv == "*") {
        if (!n->expr->is_assignable) {
          assert(false);
        }
        if (typeid(*n->expr->eval_type) != typeid(TypePtr)) {
          assert(false);
        }
        pop("r10", ctx, code);
        eval(n->expr, "r10", code);
        push("r10", ctx, code);
        std::shared_ptr<TypePtr> expr_type = std::dynamic_pointer_cast<TypePtr>(n->expr->eval_type);
        n->eval_type = expr_type->of;
        n->is_assignable = true;
        return;
      }
      if (n->op->sv == "&") {
        if (!n->expr->is_assignable) {
          assert(false);
        }
        n->eval_type = std::make_shared<TypePtr>(n->expr->eval_type);
        n->is_assignable = false;
        return;
      }
      assert(false);
    }
    if (typeid(*ast) == typeid(ASTFuncCallExpr)) {
      std::shared_ptr<ASTFuncCallExpr> n = std::dynamic_pointer_cast<ASTFuncCallExpr>(ast);
      generate_text_section(n->primary, ctx, code);
      if (typeid(*n->primary->eval_type) != typeid(TypeFunc)) {
        // TODO error
        assert(false);
      }
      if (n->args.size() > 6) {
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
        pop(param_reg_names[i], ctx, code);
        if (typeid(*n->args[i]->eval_type) != typeid(TypeArray)) {
          eval(n->args[i], param_reg_names[i], code);
        }
      }
      pop("rax", ctx, code);
      eval(n->primary, "rax", code);
      call("rax", ctx, code);
      push("rax", ctx, code);
      n->eval_type = tf->ret_type;
      n->is_assignable = false;
      return;
    }
    if (typeid(*ast) == typeid(ASTArrayAccessExpr)) {
      std::shared_ptr<ASTArrayAccessExpr> n = std::dynamic_pointer_cast<ASTArrayAccessExpr>(ast);
      generate_text_section(n->primary, ctx, code);
      if (typeid(*n->primary->eval_type) != typeid(TypeArray)) {
        // TODO error
        assert(false);
      }
      std::shared_ptr<TypeArray> a = std::dynamic_pointer_cast<TypeArray>(n->primary->eval_type);
      generate_text_section(n->expr, ctx, code);
      if (typeid(*n->expr->eval_type) != typeid(TypeNum)) {
        assert(false);
      }
      pop("r11", ctx, code);
      eval(n->expr, "r11", code);
      code += "imul r11, " + std::to_string(a->of->size) + "\n";
      pop("r10", ctx, code);
      code += "add r10, r11\n";
      push("r10", ctx, code);
      n->eval_type = a->of;
      n->is_assignable = true;
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
        std::shared_ptr<LocalVar> lv = ctx->get_local_var(n->op->sv);
        if (lv) {
          get_local_var_addr("r10", lv, code);
          push("r10", ctx, code);
          n->eval_type = lv->type;
          n->is_assignable = true;
          return;
        }
        std::shared_ptr<GlobalVar> gv = ctx->get_global_var(n->op->sv);
        if (gv) {
          get_global_var_addr("r10", gv, code);
          push("r10", ctx, code);
          n->eval_type = gv->type;
          n->is_assignable = true;
          return;
        }
        std::shared_ptr<Func> f = ctx->get_func(n->op->sv);
        if (f) {
          get_func_addr("r10", f, code);
          push("r10", ctx, code);
          n->eval_type = f->type;
          n->is_assignable = false;
          return;
        }
        // TODO get extern
        code += ".global " + std::string(n->op->sv) + "\n";
        code += "mov r10, [rip + " + std::string(n->op->sv) + "@GOTPCREL]\n";
        push("r10", ctx, code);
        n->eval_type = std::make_shared<TypeFunc>(std::make_shared<TypeAny>());
        n->is_assignable = false;
        return;
      } else if (n->op->type == NumberConstant) {
        code += "mov r10, " + std::string(n->op->sv) + "\n";
        push("r10", ctx, code);
        n->eval_type = std::make_shared<TypeNum>();
        n->is_assignable = false;
        return;
      } else if (n->op->type == StringLiteral) {
        std::string label = create_label();
        get_str_addr("r10", label, code);
        push("r10", ctx, code);
        ctx->add_str(label, n->op->sv);
        n->eval_type = std::make_shared<TypeArray>(
          std::make_shared<TypeChar>(), string_literal_length(n->op->sv)
        );
        n->is_assignable = true;
        return;
      }
      assert(false);
    }
  }

  void generate_data_section(std::shared_ptr<Context> ctx, std::string &code) {
    code += ".data\n";
    for (std::pair<std::string, std::string> &d: ctx->strs) {
      code += ".align 8\n";
      code += d.first + ": .asciz " + d.second + "\n";
    }
    for (auto &p: ctx->global_vars) {
      code += ".align 8\n";
      code += ".global " + p.first + "\n";
      code += p.first + ":\n";
      code += ".byte ";
      for (int i = 0; i < p.second->type->size; i++) {
        code += "0";
        code += ((i == p.second->type->size - 1) ? "\n" : ", ");
      }
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