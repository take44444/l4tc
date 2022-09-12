#include "./generator.hpp"

namespace generator {
  bool init_generator(std::shared_ptr<ASTTranslationUnit> tu, std::shared_ptr<Context> ctx, GError &err) {
    std::vector<std::shared_ptr<ASTStructDef>> sds;
    for (std::shared_ptr<AST> d: tu->external_declarations) {
      if (typeid(*d) == typeid(ASTStructDef)) {
        sds.push_back(std::dynamic_pointer_cast<ASTStructDef>(d));
      }
    }
    std::vector<std::shared_ptr<ASTStructDef>> ordered_sds = is_dag(sds, err);
    if (ordered_sds.size() != sds.size()) return false;
    if (!ctx->create_and_add_struct_types(ordered_sds, err)) return false;
    for (std::shared_ptr<AST> d: tu->external_declarations) {
      if (typeid(*d) == typeid(ASTFuncDef)) {
        std::shared_ptr<ASTFuncDef> n = std::dynamic_pointer_cast<ASTFuncDef>(d);
        std::shared_ptr<ASTFuncDeclaration> fd = n->declaration;
        std::shared_ptr<TypeFunc> tf = ctx->create_func_type(fd, err);
        if (!tf) return false;
        if (typeid(*tf->ret_type) == typeid(TypeArray)) {
          err = GError(
            "array cannot be return type",
            fd->declarator->declarator->op
          );
          return false;
        }
        if (typeid(*tf->ret_type) == typeid(TypeStruct)) {
          err = GError(
            "struct cannot be return type",
            fd->declarator->declarator->op
          );
          return false;
        }
        if (fd->declarator->args.size() > 6) {
          err = GError(
            "the maximum number of args is 6, but it is " +
            std::to_string(fd->declarator->args.size()),
            fd->declarator->op
          );
          return false;
        }
        ctx->add_func(std::string(fd->declarator->declarator->op->sv), tf);
        continue;
      }
    }
    return true;
  }
  bool generate_text_section(std::shared_ptr<AST> ast, std::shared_ptr<Context> ctx, std::string &code, GError &err) {
    if (typeid(*ast) == typeid(ASTTranslationUnit)) {
      std::shared_ptr<ASTTranslationUnit> n = std::dynamic_pointer_cast<ASTTranslationUnit>(ast);
      if (!init_generator(n, ctx, err)) return false;
      code += ".intel_syntax noprefix\n"; // use intel syntax
      code += ".text\n"; // text section
      code += ".global main\n";
      for (std::shared_ptr<AST> d: n->external_declarations) {
        if (!generate_text_section(d, ctx, code, err)) return false;
      }
      return true;
    }
    // declaration-spec simple-declarators
    if (typeid(*ast) == typeid(ASTExternalDeclaration)) {
      std::shared_ptr<ASTExternalDeclaration> n = std::dynamic_pointer_cast<ASTExternalDeclaration>(ast);
      std::shared_ptr<EvalType> type = ctx->create_type(n->declaration_spec, err);
      if (!type) return false;
      for (std::shared_ptr<ASTDeclarator> d: n->declarators) {
        ctx->add_global_var(std::string(d->op->sv), type);
      }
      return true;
    }
    if (typeid(*ast) == typeid(ASTFfi)) {
      std::shared_ptr<ASTFfi> n = std::dynamic_pointer_cast<ASTFfi>(ast);
      ctx->add_ffi(std::string(n->declarator->op->sv));
      return true;
    }
    if (typeid(*ast) == typeid(ASTStructDef)) {
      return true;
    }
    if (typeid(*ast) == typeid(ASTFuncDef)) {
      std::shared_ptr<ASTFuncDef> n = std::dynamic_pointer_cast<ASTFuncDef>(ast);
      if (typeid(*n->body->items.back()) != typeid(ASTReturnStmt)) {
        err = GError(
          "missing return statement at the end of function",
          n->declaration->declarator->declarator->op
        );
        return false;
      }
      std::string func_name = std::string(
        n->declaration->declarator->declarator->op->sv
      );
      std::shared_ptr<Func> f = ctx->get_func(func_name);
      assert(f);
      ctx->ret_type = f->type->ret_type;
      code += func_name + ":\n";
      code += "push rbp\n";
      code += "mov rbp, rsp\n";
      ctx->rsp = 0; // now rsp == rbp
      ctx->start_scope();
      for (int i=0; i < (int)f->type->type_args.size(); i++) {
        push_args(i, f->type->type_args[i], ctx, code);
        ctx->add_local_var(
          std::string(n->declaration->declarator->args[i]->declarator->op->sv),
          f->type->type_args[i]
        );
      }
      if (!generate_text_section(n->body, ctx, code, err)) return false;
      ctx->end_scope();
      ret(code);
      return true;
    }
    if (typeid(*ast) == typeid(ASTDeclaration)) {
      std::shared_ptr<ASTDeclaration> n = std::dynamic_pointer_cast<ASTDeclaration>(ast);
      std::shared_ptr<EvalType> type = ctx->create_type(n->declaration_spec, err);
      if (!type) return false;
      for (std::shared_ptr<ASTDeclarator> d: n->declarators) {
        code += "sub rsp, " + std::to_string(align_8(type->size)) + "\n";
        ctx->rsp -= align_8(type->size);
        ctx->add_local_var(std::string(d->op->sv), type);
      }
      return true;
    }
    if (typeid(*ast) == typeid(ASTIfStmt)) {
      std::shared_ptr<ASTIfStmt> n = std::dynamic_pointer_cast<ASTIfStmt>(ast);
      std::string false_label = create_label();
      std::string end_label = create_label();
      if (!generate_text_section(n->cond, ctx, code, err)) return false;
      if (typeid(*n->cond->eval_type) == typeid(TypeArray) ||
          typeid(*n->cond->eval_type) == typeid(TypeStruct)) {
        err = GError(
          n->cond->eval_type->to_string() +
          " cannot be evaluated as bool",
          n->op
        );
        return false;
      }
      pop("r10", ctx, code);
      eval(n->cond, "r10", code);
      code += "cmp r10, 0\n";
      code += "jz " + false_label + "\n";
      if (!generate_text_section(n->true_stmt, ctx, code, err)) return false;
      code += "jmp " + end_label + "\n";
      code += false_label + ":\n";
      if (n->false_stmt) {
        if (!generate_text_section(n->false_stmt, ctx, code, err)) return false;
      }
      code += end_label + ":\n";
      return true;
    }
    if (typeid(*ast) == typeid(ASTElseStmt)) {
      std::shared_ptr<ASTElseStmt> n = std::dynamic_pointer_cast<ASTElseStmt>(ast);
      std::string false_label = create_label();
      std::string end_label = create_label();
      if (n->cond) {
        if (!generate_text_section(n->cond, ctx, code, err)) return false;
        if (typeid(*n->cond->eval_type) == typeid(TypeArray) ||
            typeid(*n->cond->eval_type) == typeid(TypeStruct)) {
          err = GError(
            n->cond->eval_type->to_string() +
            " cannot be evaluated as bool",
            n->op
          );
          return false;
        }
        pop("r10", ctx, code);
        eval(n->cond, "r10", code);
        code += "cmp r10, 0\n";
        code += "jnz " + false_label + "\n";
      }
      if (!generate_text_section(n->true_stmt, ctx, code, err)) return false;
      code += "jmp " + end_label + "\n";
      code += false_label + ":\n";
      if (n->false_stmt) {
        if (!generate_text_section(n->false_stmt, ctx, code, err)) return false;
      }
      code += end_label + ":\n";
      return true;
    }
    if (typeid(*ast) == typeid(ASTLoopStmt)) {
      std::shared_ptr<ASTLoopStmt> n = std::dynamic_pointer_cast<ASTLoopStmt>(ast);
      std::string loop_label = create_label();
      std::string end_label = create_label();
      ctx->start_loop(loop_label, end_label);
      code += loop_label + ":\n";
      if (!generate_text_section(n->cond, ctx, code, err)) return false;
      if (typeid(*n->cond->eval_type) == typeid(TypeArray) ||
          typeid(*n->cond->eval_type) == typeid(TypeStruct)) {
        err = GError(
          n->cond->eval_type->to_string() +
          " cannot be evaluated as bool",
          n->op
        );
        return false;
      }
      pop("r10", ctx, code);
      eval(n->cond, "r10", code);
      code += "cmp r10, 0\n";
      code += "jz " + end_label + "\n";
      if (!generate_text_section(n->true_stmt, ctx, code, err)) return false;
      code += "jmp " + loop_label + "\n";
      code += end_label + ":\n";
      ctx->end_loop();
      return true;
    }
    if (typeid(*ast) == typeid(ASTCompoundStmt)) {
      std::shared_ptr<ASTCompoundStmt> n = std::dynamic_pointer_cast<ASTCompoundStmt>(ast);
      ctx->start_scope();
      int saved_rsp = ctx->rsp;
      for (std::shared_ptr<AST> stmt: n->items) {
        if (typeid(*stmt) == typeid(ASTDeclaration)) {
          if (!generate_text_section(stmt, ctx, code, err)) return false;
        }
      }
      for (std::shared_ptr<AST> stmt: n->items) {
        if (typeid(*stmt) == typeid(ASTDeclaration)) continue;
        if (!generate_text_section(stmt, ctx, code, err)) return false;
      }
      ctx->rsp = saved_rsp;
      code += "lea rsp, [rbp - " + std::to_string(-saved_rsp) + "]\n";
      ctx->end_scope();
      return true;
    }
    if (typeid(*ast) == typeid(ASTExprStmt)) {
      std::shared_ptr<ASTExprStmt> n = std::dynamic_pointer_cast<ASTExprStmt>(ast);
      if (!generate_text_section(n->expr, ctx, code, err)) return false;
      pop("r10", ctx, code);
      return true;
    }
    if (typeid(*ast) == typeid(ASTBreakStmt)) {
      std::shared_ptr<ASTBreakStmt> n = std::dynamic_pointer_cast<ASTBreakStmt>(ast);
      std::shared_ptr<Loop> loop = ctx->get_loop();
      if (!loop) {
        err = GError(
          "found break statement out of loop",
          n->op
        );
      }
      code += "jmp " + loop->end_label + "\n";
      return true;
    }
    if (typeid(*ast) == typeid(ASTContinueStmt)) {
      std::shared_ptr<ASTContinueStmt> n = std::dynamic_pointer_cast<ASTContinueStmt>(ast);
      std::shared_ptr<Loop> loop = ctx->get_loop();
      if (!loop) {
        err = GError(
          "found continue statement out of loop",
          n->op
        );
      }
      code += "jmp " + loop->loop_label + "\n";
      return true;
    }
    if (typeid(*ast) == typeid(ASTReturnStmt)) {
      std::shared_ptr<ASTReturnStmt> n = std::dynamic_pointer_cast<ASTReturnStmt>(ast);
      std::shared_ptr<ASTExpr> expr = std::dynamic_pointer_cast<ASTExpr>(n->expr);
      if (!generate_text_section(expr, ctx, code, err)) return false;
      if (!type_eq(ctx->ret_type, expr->eval_type)) {
        err = GError(
          "expect return type " + ctx->ret_type->to_string() +
          ", but returning " + expr->eval_type->to_string(),
          n->op
        );
        return false;
      }
      pop("rax", ctx, code);
      eval(expr, "rax", code);
      ret(code);
      return true;
    }
    if (typeid(*ast) == typeid(ASTAssignExpr)) {
      std::shared_ptr<ASTAssignExpr> n = std::dynamic_pointer_cast<ASTAssignExpr>(ast);
      if (!generate_text_section(n->right, ctx, code, err)) return false;
      if (!generate_text_section(n->left, ctx, code, err)) return false;
      if (!type_eq(n->left->eval_type, n->right->eval_type)) {
        err = GError(
          "cannot assign " + n->right->eval_type->to_string() +
          " to " + n->left->eval_type->to_string(),
          n->op
        );
        return false;
      }
      if (!try_assign(n->left, n->right, ctx, code)) {
        err = GError("the left is not assinable", n->op);
        return false;
      }

      n->eval_type = n->left->eval_type;
      n->is_assignable = false;
      return true;
    }
    if (typeid(*ast) == typeid(ASTLogicalOrExpr)) {
      std::shared_ptr<ASTLogicalOrExpr> n = std::dynamic_pointer_cast<ASTLogicalOrExpr>(ast);
      std::string label = create_label();
      if (!generate_text_section(n->left, ctx, code, err)) return false;
      if (typeid(*n->left->eval_type) == typeid(TypeArray) ||
          typeid(*n->left->eval_type) == typeid(TypeStruct)) {
        err = GError(
          "the operator is not avalilable with " +
          n->left->eval_type->to_string(),
          n->op
        );
        return false;
      }
      pop("r10", ctx, code);
      eval(n->left, "r10", code);
      code += "cmp r10, 0\n";
      code += "setnz r10b\n";
      code += "movzx r10, r10b\n";
      code += "jnz " + label + "\n";
      if (!generate_text_section(n->right, ctx, code, err)) return false;
      if (typeid(*n->right->eval_type) == typeid(TypeArray) ||
          typeid(*n->right->eval_type) == typeid(TypeStruct)) {
        err = GError(
          "the operator is not avalilable with " +
          n->left->eval_type->to_string(),
          n->op
        );
        return false;
      }
      pop("r10", ctx, code);
      eval(n->right, "r10", code);
      code += "cmp r10, 0\n";
      code += "setnz r10b\n";
      code += "movzx r10, r10b\n";
      code += label + ":\n";
      push("r10", ctx, code);
      n->eval_type = std::make_shared<TypeNum>();
      n->is_assignable = false;
      return true;
    }
    if (typeid(*ast) == typeid(ASTLogicalAndExpr)) {
      std::shared_ptr<ASTLogicalAndExpr> n = std::dynamic_pointer_cast<ASTLogicalAndExpr>(ast);
      std::string label = create_label();
      if (!generate_text_section(n->left, ctx, code, err)) return false;
      if (typeid(*n->left->eval_type) == typeid(TypeArray) ||
          typeid(*n->left->eval_type) == typeid(TypeStruct)) {
        err = GError(
          "the operator is not avalilable with " +
          n->left->eval_type->to_string(),
          n->op
        );
        return false;
      }
      pop("r10", ctx, code);
      eval(n->left, "r10", code);
      code += "cmp r10, 0\n";
      code += "setnz r10b\n";
      code += "movzx r10, r10b\n";
      code += "jz " + label + "\n";
      if (!generate_text_section(n->right, ctx, code, err)) return false;
      if (typeid(*n->right->eval_type) == typeid(TypeArray) ||
          typeid(*n->right->eval_type) == typeid(TypeStruct)) {
        err = GError(
          "the operator is not avalilable with " +
          n->left->eval_type->to_string(),
          n->op
        );
        return false;
      }
      pop("r10", ctx, code);
      eval(n->right, "r10", code);
      code += "cmp r10, 0\n";
      code += "setnz r10b\n";
      code += "movzx r10, r10b\n";
      code += label + ":\n";
      push("r10", ctx, code);
      n->eval_type = std::make_shared<TypeNum>();
      n->is_assignable = false;
      return true;
    }
    // if (typeid(*ast) == typeid(ASTBitwiseOrExpr)) {}
    // if (typeid(*ast) == typeid(ASTBitwiseXorExpr)) {}
    // if (typeid(*ast) == typeid(ASTBitwiseAndExpr)) {}
    if (typeid(*ast) == typeid(ASTEqualityExpr)) {
      std::shared_ptr<ASTEqualityExpr> n = std::dynamic_pointer_cast<ASTEqualityExpr>(ast);
      if (!generate_text_section(n->left, ctx, code, err)) return false;
      if (!generate_text_section(n->right, ctx, code, err)) return false;
      if (!type_eq(n->left->eval_type, n->right->eval_type)) {
        err = GError(
          "cannot compare " + n->right->eval_type->to_string() +
          " with " + n->left->eval_type->to_string(),
          n->op
        );
        return false;
      }
      if (typeid(*n->left->eval_type) == typeid(TypeArray) ||
          typeid(*n->left->eval_type) == typeid(TypeStruct)) {
        err = GError(
          "the operator is not avalilable with " +
          n->left->eval_type->to_string(),
          n->op
        );
        return false;
      }
      pop("r11", ctx, code);
      pop("r10", ctx, code);
      eval(n->left, "r10", code);
      eval(n->right, "r11", code);
      if (n->op->sv == "=") cmp("r10", "r10", "r11", "e", code);
      else cmp("r10", "r10", "r11", "ne", code);
      push("r10", ctx, code);
      n->eval_type = std::make_shared<TypeNum>();
      n->is_assignable = false;
      return true;
    }
    if (typeid(*ast) == typeid(ASTRelationalExpr)) {
      std::shared_ptr<ASTRelationalExpr> n = std::dynamic_pointer_cast<ASTRelationalExpr>(ast);
      if (!generate_text_section(n->left, ctx, code, err)) return false;
      if (!generate_text_section(n->right, ctx, code, err)) return false;
      if (typeid(*n->left->eval_type) != typeid(TypeNum)) {
        err = GError(
          "the operator is not avalilable with " +
          n->left->eval_type->to_string(),
          n->op
        );
        return false;
      }
      if (typeid(*n->right->eval_type) != typeid(TypeNum)) {
        err = GError(
          "the operator is not avalilable with " +
          n->right->eval_type->to_string(),
          n->op
        );
        return false;
      }
      pop("r11", ctx, code);
      pop("r10", ctx, code);
      eval(n->left, "r10", code);
      eval(n->right, "r11", code);
      if (n->op->sv == ">") cmp("r10", "r10", "r11", "g", code);
      else if (n->op->sv == "<") cmp("r10", "r10", "r11", "l", code);
      else if (n->op->sv == ">=") cmp("r10", "r10", "r11", "ge", code);
      else cmp("r10", "r10", "r11", "le", code);
      push("r10", ctx, code);
      n->eval_type = std::make_shared<TypeNum>();
      n->is_assignable = false;
      return true;
    }
    // if (typeid(*ast) == typeid(ASTShiftExpr)) {}
    if (typeid(*ast) == typeid(ASTAdditiveExpr)) {
      std::shared_ptr<ASTAdditiveExpr> n = std::dynamic_pointer_cast<ASTAdditiveExpr>(ast);
      if (!generate_text_section(n->left, ctx, code, err)) return false;
      if (!generate_text_section(n->right, ctx, code, err)) return false;
      if (typeid(*n->left->eval_type) != typeid(TypeNum)) {
        err = GError(
          "the operator is not avalilable with " +
          n->left->eval_type->to_string(),
          n->op
        );
        return false;
      }
      if (typeid(*n->right->eval_type) != typeid(TypeNum)) {
        err = GError(
          "the operator is not avalilable with " +
          n->right->eval_type->to_string(),
          n->op
        );
        return false;
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
      return true;
    }
    if (typeid(*ast) == typeid(ASTMultiplicativeExpr)) {
      std::shared_ptr<ASTMultiplicativeExpr> n = std::dynamic_pointer_cast<ASTMultiplicativeExpr>(ast);
      if (!generate_text_section(n->left, ctx, code, err)) return false;
      if (!generate_text_section(n->right, ctx, code, err)) return false;
      if (typeid(*n->left->eval_type) != typeid(TypeNum)) {
        err = GError(
          "the operator is not avalilable with " +
          n->left->eval_type->to_string(),
          n->op
        );
        return false;
      }
      if (typeid(*n->right->eval_type) != typeid(TypeNum)) {
        err = GError(
          "the operator is not avalilable with " +
          n->right->eval_type->to_string(),
          n->op
        );
        return false;
      }
      code += "xor rdx, rdx\n";
      pop("r10", ctx, code);
      pop("rax", ctx, code);
      eval(n->left, "rax", code);
      eval(n->right, "r10", code);
      if (n->op->sv == "*") code += "imul r10\n";
      else if (n->op->sv == "/") code += "idiv r10\n";
      if (n->op->sv == "%") {
        code += "idiv r10\n";
        code += "mov r10, rdx\n";
      } else {
        code += "mov r10, rax\n";
      }
      push("r10", ctx, code);
      n->eval_type = n->left->eval_type;
      n->is_assignable = false;
      return true;
    }
    if (typeid(*ast) == typeid(ASTUnaryExpr)) {
      std::shared_ptr<ASTUnaryExpr> n = std::dynamic_pointer_cast<ASTUnaryExpr>(ast);
      if (!generate_text_section(n->expr, ctx, code, err)) return false;
      if (n->op->sv == "*") {
        if (!n->expr->is_assignable) {
          err = GError("cannot derefer unassignable expr", n->op);
          return false;
        }
        if (typeid(*n->expr->eval_type) != typeid(TypePtr)) {
          err = GError("cannot derefer non-pointer expr", n->op);
          return false;
        }
        pop("r10", ctx, code);
        eval(n->expr, "r10", code);
        push("r10", ctx, code);
        std::shared_ptr<TypePtr> expr_type = std::dynamic_pointer_cast<TypePtr>(n->expr->eval_type);
        n->eval_type = expr_type->of;
        n->is_assignable = true;
        return true;
      }
      if (n->op->sv == "&") {
        if (!n->expr->is_assignable) {
          err = GError("cannot refer unassignable expr", n->op);
          return false;
        }
        n->eval_type = std::make_shared<TypePtr>(n->expr->eval_type);
        n->is_assignable = false;
        return true;
      }
      assert(false);
      return false;
    }
    if (typeid(*ast) == typeid(ASTFuncCallExpr)) {
      std::shared_ptr<ASTFuncCallExpr> n = std::dynamic_pointer_cast<ASTFuncCallExpr>(ast);
      if (!generate_text_section(n->primary, ctx, code, err)) return false;
      if (typeid(*n->primary->eval_type) != typeid(TypeFunc) &&
          typeid(*n->primary->eval_type) != typeid(TypeFfi)) {
        err = GError(
          n->primary->eval_type->to_string() +
          " is not function-call-expr",
          n->op
        );
        return false;
      }
      if (n->args.size() > 6) {
        err = GError(
          "the maximum number of args is 6, but it is " +
          std::to_string(n->args.size()),
          n->op
        );
        return false;
      }
      for (int i=0; i < (int)n->args.size(); i++) {
        if (!generate_text_section(n->args[i], ctx, code, err)) return false;
      }
      if (typeid(*n->primary->eval_type) == typeid(TypeFunc)) {
        std::shared_ptr<TypeFunc> tf = std::dynamic_pointer_cast<TypeFunc>(n->primary->eval_type);
        if (n->args.size() != tf->type_args.size()) {
          err = GError(
            "the type of function is " + tf->to_string() +
            ", but the number of args is " + std::to_string(n->args.size()),
            n->op
          );
          return false;
        }
        for (int i=0; i < (int)n->args.size(); i++) {
          if (!type_eq(n->args[i]->eval_type, tf->type_args[i])) {
            err = GError(
              "the type of function is " + tf->to_string(),
              n->op
            );
            return false;
          }
        }
        n->eval_type = tf->ret_type;
      } else {
        n->eval_type = n->primary->eval_type;
      }
      for (int i=(int)n->args.size()-1; i >= 0; i--) {
        pop(param_reg_names[i], ctx, code);
        if (typeid(*n->args[i]->eval_type) != typeid(TypeArray) &&
            typeid(*n->args[i]->eval_type) != typeid(TypeStruct)) {
          eval(n->args[i], param_reg_names[i], code);
        }
      }
      pop("rax", ctx, code);
      eval(n->primary, "rax", code);
      call("rax", ctx, code);
      push("rax", ctx, code);
      n->is_assignable = false;
      return true;
    }
    if (typeid(*ast) == typeid(ASTArrayAccessExpr)) {
      std::shared_ptr<ASTArrayAccessExpr> n = std::dynamic_pointer_cast<ASTArrayAccessExpr>(ast);
      if (!generate_text_section(n->primary, ctx, code, err)) return false;
      if (typeid(*n->primary->eval_type) != typeid(TypeArray)) {
        err = GError(
          n->primary->eval_type->to_string() + " is not array",
          n->op
        );
        return false;
      }
      std::shared_ptr<TypeArray> a = std::dynamic_pointer_cast<TypeArray>(n->primary->eval_type);
      if (!generate_text_section(n->expr, ctx, code, err)) return false;
      if (typeid(*n->expr->eval_type) != typeid(TypeNum)) {
        err = GError(
          n->expr->eval_type->to_string() + " is not num",
          n->op
        );
        return false;
      }
      pop("r11", ctx, code);
      eval(n->expr, "r11", code);
      code += "imul r11, " + std::to_string(a->of->size) + "\n";
      pop("r10", ctx, code);
      code += "add r10, r11\n";
      push("r10", ctx, code);
      n->eval_type = a->of;
      n->is_assignable = true;
      return true;
    }
    if (typeid(*ast) == typeid(ASTStructAccessExpr)) {
      std::shared_ptr<ASTStructAccessExpr> n = std::dynamic_pointer_cast<ASTStructAccessExpr>(ast);
      if (!generate_text_section(n->primary, ctx, code, err)) return false;
      if (typeid(*n->primary->eval_type) != typeid(TypeStruct)) {
        err = GError(
          "cannot solve this access because " +
          n->primary->eval_type->to_string() + " is not struct",
          n->op
        );
        return false;
      }
      std::shared_ptr<TypeStruct> s = std::dynamic_pointer_cast<TypeStruct>(n->primary->eval_type);
      std::pair<int, std::shared_ptr<EvalType>> m = s->get_member(n->op->sv);
      if (!m.second) {
        err = GError(
          "cannot find member " + std::string(n->op->sv),
          n->op
        );
        return false;
      }
      pop("r10", ctx, code);
      code += "add r10, " + std::to_string(m.first) + "\n";
      push("r10", ctx, code);
      n->eval_type = m.second;
      n->is_assignable = true;
      return true;
    }
    if (typeid(*ast) == typeid(ASTPrimaryExpr)) {
      std::shared_ptr<ASTPrimaryExpr> n = std::dynamic_pointer_cast<ASTPrimaryExpr>(ast);
      if (!generate_text_section(n->expr, ctx, code, err)) return false;
      n->eval_type = n->expr->eval_type;
      n->is_assignable = n->expr->is_assignable;
      return true;
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
          return true;
        }
        std::shared_ptr<GlobalVar> gv = ctx->get_global_var(n->op->sv);
        if (gv) {
          get_global_var_addr("r10", gv, code);
          push("r10", ctx, code);
          n->eval_type = gv->type;
          n->is_assignable = true;
          return true;
        }
        std::shared_ptr<Func> f = ctx->get_func(n->op->sv);
        if (f) {
          get_func_addr("r10", f, code);
          push("r10", ctx, code);
          n->eval_type = f->type;
          n->is_assignable = false;
          return true;
        }
        std::shared_ptr<Ffi> ff = ctx->get_ffi(n->op->sv);
        if (ff) {
          get_ffi_addr("r10", ff, code);
          push("r10", ctx, code);
          n->eval_type = ff->type;
          n->is_assignable = false;
          return true;
        }
        err = GError(
          std::string(n->op->sv) +
          " is not defined",
          n->op
        );
        return false;
      } else if (n->op->type == NumberConstant) {
        code += "mov r10, " + std::string(n->op->sv) + "\n";
        push("r10", ctx, code);
        n->eval_type = std::make_shared<TypeNum>();
        n->is_assignable = false;
        return true;
      } else if (n->op->type == StringLiteral) {
        std::string label = create_label();
        get_str_addr("r10", label, code);
        push("r10", ctx, code);
        ctx->add_str(label, n->op->sv);
        n->eval_type = std::make_shared<TypeArray>(
          std::make_shared<TypeChar>(), string_literal_length(n->op->sv)
        );
        n->is_assignable = true;
        return true;
      }
      assert(false);
      return false;
    }
    assert(false);
    return false;
  }

  void generate_data_section(std::shared_ptr<Context> ctx, std::string &code) {
    code += ".data\n";
    for (std::pair<std::string, std::string> &d: ctx->strs) {
      code += ".align 8\n";
      code += d.first + ": .asciz " + d.second + "\n";
    }
    for (auto &p: ctx->global_vars) {
      code += ".align 8\n";
      code += p.first + ":\n";
      code += ".byte ";
      for (int i = 0; i < p.second->type->size; i++) {
        code += "0";
        code += ((i == p.second->type->size - 1) ? "\n" : ", ");
      }
    }
  }

  std::string generate(std::shared_ptr<AST> ast, GError &err) {
    std::string ret;
    std::shared_ptr<Context> context = std::make_shared<Context>();
    if (!generate_text_section(ast, context, ret, err)) return "";
    generate_data_section(context, ret);
    return ret;
  }
}