#include "lupc.h"

shared_ptr<ASTTypeSpec> parse_type_spec(Token **next, Error &err) {
  // NULL check
  if (!*next) {
    err = Error("expected type-specifier, found EOF");
    return nullptr;
  }
  Token *t;
  if (
    !!(t = consume_token_with_type(next, KwNum)) ||
    !!(t = consume_token_with_type(next, KwStr)) ||
    !!(t = consume_token_with_type(next, KwVoid))
  ) {
    return make_shared<ASTTypeSpec>(t);
  }
  err = Error("expected type-specifier, found ????");
  return nullptr;
}

shared_ptr<ASTTypeSpec> parse_declaration_spec(Token **next, Error &err) {
  return parse_type_spec(next, err);
}

shared_ptr<ASTExpr> parse_expr(Token **next, Error &err);
shared_ptr<ASTExpr> parse_primary_expr(Token **next, Error &err) {
  // NULL check
  if (!(*next)) {
    err = Error("expected primary-expr, found EOF");
    return nullptr;
  }
  Token *t;
  if (
    !!(t = consume_token_with_type(next, NumberConstant)) ||
    !!(t = consume_token_with_type(next, Ident))
  ) {
    return make_shared<ASTSimpleExpr>(t);
  }
  if (!!consume_token_with_str(next, "(")) {
    shared_ptr<ASTPrimaryExpr> ret = make_shared<ASTPrimaryExpr>();
    if (!(ret->expr = parse_expr(next, err))) return nullptr;
    if (!!expect_token_with_str(next, err, ")")) return ret;
  }
  err = Error("expected primary-expr, found ????");
  return nullptr;
}

shared_ptr<ASTExpr> parse_assign_expr(Token **next, Error &err);
shared_ptr<ASTExpr> parse_postfix_expr(Token **next, Error &err) {
// function-call
// array
// increment
// decrement
// ->
// .
  shared_ptr<ASTExpr> primary = parse_primary_expr(next, err);
  if (!primary) return nullptr;
  // if "(" is not here, it is not function-call
  // but it is correct primary expr
  if (!consume_token_with_str(next, "(")) return primary;
  // now it is assignment-expr
  shared_ptr<ASTFuncCallExpr> ret = make_shared<ASTFuncCallExpr>();
  ret->primary = primary;

  shared_ptr<ASTExpr> arg;
  while (!consume_token_with_str(next, ")")) {
    if (!(arg = parse_assign_expr(next, err))) return nullptr;
    ret->args.push_back(arg);
    if (!!consume_token_with_str(next, ",")) continue;
    // end
    if (!expect_token_with_str(next, err, ")")) return nullptr;
    break;
  }
  return ret;
}

shared_ptr<ASTExpr> parse_unary_expr(Token **next, Error &err) {
  return parse_postfix_expr(next, err);
}

shared_ptr<ASTExpr> parse_multiplicative_expr(Token **next, Error &err) {
  shared_ptr<ASTExpr> left, ret = parse_unary_expr(next, err);
  if (!ret) return nullptr;
  while (
    !!consume_token_with_str(next, "*") ||
    !!consume_token_with_str(next, "/") ||
    !!consume_token_with_str(next, "%")
  ) {
    left = ret;
    ret = make_shared<ASTMultiplicativeExpr>();
    ret->left = left;
    if (!(ret->right = parse_unary_expr(next, err))) return nullptr;
  }
  return ret;
}

shared_ptr<ASTExpr> parse_additive_expr(Token **next, Error &err) {
  shared_ptr<ASTExpr> left, ret = parse_multiplicative_expr(next, err);
  if (!ret) return nullptr;
  while (
    !!consume_token_with_str(next, "+") ||
    !!consume_token_with_str(next, "-")
  ) {
    left = ret;
    ret = make_shared<ASTAdditiveExpr>();
    ret->left = left;
    if (!(ret->right = parse_multiplicative_expr(next, err))) return nullptr;
  }
  return ret;
}

shared_ptr<ASTExpr> parse_shift_expr(Token **next, Error &err) {
  shared_ptr<ASTExpr> left, ret = parse_additive_expr(next, err);
  if (!ret) return nullptr;
  while (
    !!consume_token_with_str(next, "<<") ||
    !!consume_token_with_str(next, ">>")
  ) {
    left = ret;
    ret = make_shared<ASTShiftExpr>();
    ret->left = left;
    if (!(ret->right = parse_additive_expr(next, err))) return nullptr;
  }
  return ret;
}

shared_ptr<ASTExpr> parse_relational_expr(Token **next, Error &err) {
  shared_ptr<ASTExpr> left, ret = parse_shift_expr(next, err);
  if (!ret) return nullptr;
  while (
    !!consume_token_with_str(next, "<") ||
    !!consume_token_with_str(next, ">") ||
    !!consume_token_with_str(next, "<=") ||
    !!consume_token_with_str(next, ">=")
  ) {
    left = ret;
    ret = make_shared<ASTRelationalExpr>();
    ret->left = left;
    if (!(ret->right = parse_shift_expr(next, err))) return nullptr;
  }
  return ret;
}

shared_ptr<ASTExpr> parse_equality_expr(Token **next, Error &err) {
  shared_ptr<ASTExpr> left, ret = parse_relational_expr(next, err);
  if (!ret) return nullptr;
  while (
    !!consume_token_with_str(next, "==") ||
    !!consume_token_with_str(next, "!=")
  ) {
    left = ret;
    ret = make_shared<ASTEqualityExpr>();
    ret->left = left;
    if (!(ret->right = parse_relational_expr(next, err))) return nullptr;
  }
  return ret;
}

shared_ptr<ASTExpr> parse_bitwise_and_expr(Token **next, Error &err) {
  shared_ptr<ASTExpr> left, ret = parse_equality_expr(next, err);
  if (!ret) return nullptr;
  while (!!consume_token_with_str(next, "^")) {
    left = ret;
    ret = make_shared<ASTBitwiseAndExpr>();
    ret->left = left;
    if (!(ret->right = parse_equality_expr(next, err))) return nullptr;
  }
  return ret;
}

shared_ptr<ASTExpr> parse_bitwise_xor_expr(Token **next, Error &err) {
  shared_ptr<ASTExpr> left, ret = parse_bitwise_and_expr(next, err);
  if (!ret) return nullptr;
  while (!!consume_token_with_str(next, "^")) {
    left = ret;
    ret = make_shared<ASTBitwiseXorExpr>();
    ret->left = left;
    if (!(ret->right = parse_bitwise_and_expr(next, err))) return nullptr;
  }
  return ret;
}

shared_ptr<ASTExpr> parse_bitwise_or_expr(Token **next, Error &err) {
  shared_ptr<ASTExpr> left, ret = parse_bitwise_xor_expr(next, err);
  if (!ret) return nullptr;
  while (!!consume_token_with_str(next, "|")) {
    left = ret;
    ret = make_shared<ASTBitwiseOrExpr>();
    ret->left = left;
    if (!(ret->right = parse_bitwise_xor_expr(next, err))) return nullptr;
  }
  return ret;
}

shared_ptr<ASTExpr> parse_logical_and_expr(Token **next, Error &err) {
  shared_ptr<ASTExpr> left, ret = parse_bitwise_or_expr(next, err);
  if (!ret) return nullptr;
  while (!!consume_token_with_str(next, "&&")) {
    left = ret;
    ret = make_shared<ASTLogicalAndExpr>();
    ret->left = left;
    if (!(ret->right = parse_bitwise_or_expr(next, err))) return nullptr;
  }
  return ret;
}

shared_ptr<ASTExpr> parse_logical_or_expr(Token **next, Error &err) {
  shared_ptr<ASTExpr> left, ret = parse_logical_and_expr(next, err);
  if (!ret) return nullptr;
  while (!!consume_token_with_str(next, "||")) {
    left = ret;
    ret = make_shared<ASTLogicalOrExpr>();
    ret->left = left;
    if (!(ret->right = parse_logical_and_expr(next, err))) return nullptr;
  }
  return ret;
}

shared_ptr<ASTExpr> parse_assign_expr(Token **next, Error &err) {
  shared_ptr<ASTExpr> left = parse_logical_or_expr(next, err);
  // parse error
  if (!left) return nullptr;
  // if it is not unary-expr, it can't be left of assignment-expr
  // but it is correct expr
  if (!left->is_unary_expr()) return left;
  // if ":" is not here, it is not assignment-expr
  // but it is correct expr
  if (!consume_token_with_str(next, ":")) return left;
  // now it is assignment-expr
  shared_ptr<ASTAssignExpr> ret = make_shared<ASTAssignExpr>();
  ret->left = left;
  if (!(ret->right = parse_assign_expr(next, err))) return nullptr;
  return ret;
}

shared_ptr<ASTExpr> parse_expr(Token **next, Error &err) {
  return parse_assign_expr(next, err);
}

shared_ptr<ASTExprStmt> parse_expr_stmt(Token **next, Error &err) {
  shared_ptr<ASTExprStmt> ret = make_shared<ASTExprStmt>();
  if (!(ret->expr = parse_expr(next, err))) return nullptr;
  if (!expect_token_with_str(next, err, "\n")) return nullptr;
  return ret;
}

shared_ptr<ASTReturnStmt> parse_return_stmt(Token **next, Error &err) {
  if (!expect_token_with_type(next, err, KwReturn)) return nullptr;
  shared_ptr<ASTReturnStmt> ret = make_shared<ASTReturnStmt>();
  if (!(ret->expr = parse_expr(next, err))) return nullptr;
  if (!expect_token_with_str(next, err, "\n")) return nullptr;
  return ret;
}

shared_ptr<ASTDeclarator> parse_declarator(Token **next, Error &err) {
  // NULL check
  if (!*next) {
    err = Error("expected declarator, found EOF");
    return nullptr;
  }
  // token Ident should be here
  Token *t = consume_token_with_type(next, Ident);
  if (!t) {
    err = Error("expected declarator, found ????");
    return nullptr;
  }
  return make_shared<ASTDeclarator>(t);
}

shared_ptr<ASTDeclaration> parse_declaration(Token **next, Error &err) {
  shared_ptr<ASTDeclaration> ret = make_shared<ASTDeclaration>();
  if (!(ret->declaration_spec = parse_declaration_spec(next, err))) return nullptr;

  shared_ptr<ASTDeclarator> declarator;
  while (!!(declarator = parse_declarator(next, err))) {
    ret->declarators.push_back(declarator);
    if (!!consume_token_with_str(next, ",")) continue;
    // declaration end
    if (!!expect_token_with_str(next, err, "\n")) return ret;
    break;
  }
  err = Error("expected declarator, found ????");
  return nullptr;
}

shared_ptr<ASTSimpleDeclaration> parse_simple_declaration(Token **next, Error &err) {
// type-specifier declarator
  shared_ptr<ASTSimpleDeclaration> ret = make_shared<ASTSimpleDeclaration>();
  if (!(ret->type_spec = parse_type_spec(next, err))) return nullptr;
  if (!(ret->declarator = parse_declarator(next, err))) return nullptr;
  return ret;
}

shared_ptr<ASTCompoundStmt> parse_comp_stmt(Token **next, Error &err, int indents);
shared_ptr<AST> parse_stmt(Token **next, Error &err) {
  // NULL check
  if (!*next) {
    err = Error("expected statement, found EOF");
    return nullptr;
  }
  shared_ptr<AST> ret;
  if (consume_token_with_type(next, KwBreak)) {
    if (!expect_token_with_str(next, err, "\n")) return nullptr;
    ret = make_shared<ASTBreakStmt>();
  } else if (consume_token_with_type(next, KwContinue)) {
    if (!expect_token_with_str(next, err, "\n")) return nullptr;
    ret = make_shared<ASTContinueStmt>();
  } else if ((*next)->type == KwReturn) {
    ret = parse_return_stmt(next, err);
  } else {
    ret = parse_expr_stmt(next, err);
  } // TODO: if, loop
  return ret;
}

shared_ptr<ASTCompoundStmt> parse_comp_stmt(Token **next, Error &err, int indents) {
  // NULL check
  if (!*next) {
    err = Error("expected compound-stmt, found EOF");
    return nullptr;
  }
  shared_ptr<ASTCompoundStmt> ret = make_shared<ASTCompoundStmt>();
  shared_ptr<AST> item;
  while (1) {
    if (!consume_token_with_indents(next, indents)) {
      if (!*next || *((*next)->begin) != ' ' || (*next)->length < indents) {
        // compound-stmt end
        return ret;
      } else {
      // inner compound-stmt
        if (!(item = parse_comp_stmt(next, err, indents + 2))) break;
      }
    } else if (
      !(item = parse_declaration(next, err)) &&
      !(item = parse_stmt(next, err))
    ) {
      break;
    }
    ret->items.push_back(item);
  }
  return nullptr;
}

shared_ptr<ASTFuncDeclarator> parse_func_declarator(Token **next, Error &err) {
// declarator(declaration, ...)
  shared_ptr<ASTFuncDeclarator> ret = make_shared<ASTFuncDeclarator>();
  if (!(ret->declarator = parse_declarator(next, err))) return nullptr;
  if (!expect_token_with_str(next, err, "(")) return nullptr;

  shared_ptr<ASTSimpleDeclaration> declaration;
  while (!consume_token_with_str(next, ")")) {
    if (!(declaration = parse_simple_declaration(next, err))) return nullptr;
    ret->args.push_back(declaration);
    if (!!consume_token_with_str(next, ",")) continue;
    // end
    if (!expect_token_with_str(next, err, ")")) return nullptr;
    break;
  }
  return ret;
}

shared_ptr<ASTFuncDeclaration> parse_func_declaration(Token **next, Error &err) {
// func-declarator -> type
  shared_ptr<ASTFuncDeclaration> ret = make_shared<ASTFuncDeclaration>();
  if (!(ret->declarator = parse_func_declarator(next, err))) return nullptr;
  // token "->" should be here
  if (!expect_token_with_str(next, err, "->")) return nullptr;
  if (!(ret->type_spec = parse_type_spec(next, err))) return nullptr;
  // token LF should be here
  if (!expect_token_with_str(next, err, "\n")) return nullptr;
  return ret;
}

shared_ptr<ASTFuncDef> parse_func_def(Token **next, Error &err) {
// func-declaration compound-stmt
  if (!expect_token_with_type(next, err, KwFunc)) return nullptr;
  shared_ptr<ASTFuncDef> ret = make_shared<ASTFuncDef>();
  if (!(ret->declaration = parse_func_declaration(next, err))) return nullptr;
  if (!(ret->body = parse_comp_stmt(next, err, 2))) return nullptr;
  return ret;
}

shared_ptr<ASTExternalDeclaration> parse_external_declaration(Token **next, Error &err) {
  shared_ptr<ASTExternalDeclaration> ret = make_shared<ASTExternalDeclaration>();
  if (!(ret->declaration_spec = parse_declaration_spec(next, err))) return nullptr;

  shared_ptr<ASTDeclarator> declarator;
  while (!!(declarator = parse_declarator(next, err))) {
    ret->declarators.push_back(declarator);
    if (!!consume_token_with_str(next, ",")) continue;
    // declaration end
    if (!!expect_token_with_str(next, err, "\n")) return ret;
    break;
  }
  err = Error("expected declarator, found ????");
  return nullptr;
}

shared_ptr<ASTTranslationUnit> parse_translation_unit(Token **next, Error &err) {
  shared_ptr<ASTTranslationUnit> ret = make_shared<ASTTranslationUnit>();

  shared_ptr<AST> external_declaration;
  while (!!*next) { // NULL check
    if ((*next)->type == KwFunc) {
      external_declaration = parse_func_def(next, err);
    } else {
      external_declaration = parse_external_declaration(next, err);
    }
    if (!external_declaration) return nullptr;
    ret->external_declarations.push_back(external_declaration);
  }
  return ret;
}

void init_parser(Token **head_token) {
  // *head_token can be NULL
  Token *next = *head_token;
  Token *t, *bef;
  while (!!next) {
    // if next token is Delimiter, remove it and continue
    t = consume_token_with_type(&next, Delimiter);    
    if (!t) break;
    delete t;
  }
  // set new head_token
  *head_token = next;
  // remove first LF punctuator
  if (!!*head_token) {
    t = consume_token_with_str(head_token, "\n");
    if (!t) delete t;
  }
  // *head_token can be NULL
  next = *head_token;
  while (!!next) {
    bef = next;
    assert(bef->type != Delimiter);
    next = bef->next;
    while (!!next) {
      // if next token is Delimiter, remove it and continue
      t = consume_token_with_type(&next, Delimiter);
      if (!t) break;
      delete t;
    }
    bef->next = next;
  }
}

template <typename T> void print_ast_vec(vector<shared_ptr<T>> &v, int depth) {
  fprintf(stderr, "[");
  if (v.size() == 0) {
    fprintf(stderr, "]");
    return;
  }
  for (int i=0; i < (int)v.size(); i++) {
    fprintf(stderr, "%s\n", i ? "," : "");
    for (int j_ = 0; j_ < depth + 1; j_++) fputc(' ', stderr);
    print_ast_sub(v.at(i), depth + 1);
  }
  fprintf(stderr, "\n");
  for (int i_ = 0; i_ < depth; i_++) fputc(' ', stderr);
  fprintf(stderr, "]");
}

void print_ast_sub(shared_ptr<AST> n, int depth) {
  if (n->type == TypeSpec) {
    shared_ptr<ASTTypeSpec> nn = dynamic_pointer_cast<ASTTypeSpec>(n);
    fprintf(stderr, "TypeSpec<%.*s>", nn->op->length, nn->op->begin);
    return;
  }
  if (n->type == SimpleExpr) {
    shared_ptr<ASTSimpleExpr> nn = dynamic_pointer_cast<ASTSimpleExpr>(n);
    fprintf(stderr, "SimpleExpr<%.*s>", nn->op->length, nn->op->begin);
    return;
  }
  if (n->type == PrimaryExpr) {
    shared_ptr<ASTPrimaryExpr> nn = dynamic_pointer_cast<ASTPrimaryExpr>(n);
    fprintf(stderr, "PrimaryExpr(e=");
    print_ast_sub(nn->expr, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == FuncCallExpr) {
    shared_ptr<ASTFuncCallExpr> nn = dynamic_pointer_cast<ASTFuncCallExpr>(n);
    fprintf(stderr, "FuncCallExpr(p=");
    print_ast_sub(nn->primary, depth);
    fprintf(stderr, ", args=");
    print_ast_vec(nn->args, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == MultiplicativeExpr) {
    shared_ptr<ASTMultiplicativeExpr> nn = dynamic_pointer_cast<ASTMultiplicativeExpr>(n);
    fprintf(stderr, "MultiplicativeExpr(l=");
    print_ast_sub(nn->left, depth);
    fprintf(stderr, ", r=");
    print_ast_sub(nn->right, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == AdditiveExpr) {
    shared_ptr<ASTAdditiveExpr> nn = dynamic_pointer_cast<ASTAdditiveExpr>(n);
    fprintf(stderr, "AdditiveExpr(l=");
    print_ast_sub(nn->left, depth);
    fprintf(stderr, ", r=");
    print_ast_sub(nn->right, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == ShiftExpr) {
    shared_ptr<ASTShiftExpr> nn = dynamic_pointer_cast<ASTShiftExpr>(n);
    fprintf(stderr, "ShiftExpr(l=");
    print_ast_sub(nn->left, depth);
    fprintf(stderr, ", r=");
    print_ast_sub(nn->right, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == RelationalExpr) {
    shared_ptr<ASTRelationalExpr> nn = dynamic_pointer_cast<ASTRelationalExpr>(n);
    fprintf(stderr, "RelationalExpr(l=");
    print_ast_sub(nn->left, depth);
    fprintf(stderr, ", r=");
    print_ast_sub(nn->right, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == EqualityExpr) {
    shared_ptr<ASTEqualityExpr> nn = dynamic_pointer_cast<ASTEqualityExpr>(n);
    fprintf(stderr, "EqualityExpr(l=");
    print_ast_sub(nn->left, depth);
    fprintf(stderr, ", r=");
    print_ast_sub(nn->right, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == BitwiseAndExpr) {
    shared_ptr<ASTBitwiseAndExpr> nn = dynamic_pointer_cast<ASTBitwiseAndExpr>(n);
    fprintf(stderr, "BitwiseAndExpr(l=");
    print_ast_sub(nn->left, depth);
    fprintf(stderr, ", r=");
    print_ast_sub(nn->right, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == BitwiseXorExpr) {
    shared_ptr<ASTBitwiseXorExpr> nn = dynamic_pointer_cast<ASTBitwiseXorExpr>(n);
    fprintf(stderr, "BitwiseXorExpr(l=");
    print_ast_sub(nn->left, depth);
    fprintf(stderr, ", r=");
    print_ast_sub(nn->right, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == BitwiseOrExpr) {
    shared_ptr<ASTBitwiseOrExpr> nn = dynamic_pointer_cast<ASTBitwiseOrExpr>(n);
    fprintf(stderr, "BitwiseOrExpr(l=");
    print_ast_sub(nn->left, depth);
    fprintf(stderr, ", r=");
    print_ast_sub(nn->right, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == LogicalAndExpr) {
    shared_ptr<ASTLogicalAndExpr> nn = dynamic_pointer_cast<ASTLogicalAndExpr>(n);
    fprintf(stderr, "LogicalAndExpr(l=");
    print_ast_sub(nn->left, depth);
    fprintf(stderr, ", r=");
    print_ast_sub(nn->right, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == LogicalOrExpr) {
    shared_ptr<ASTLogicalOrExpr> nn = dynamic_pointer_cast<ASTLogicalOrExpr>(n);
    fprintf(stderr, "LogicalOrExpr(l=");
    print_ast_sub(nn->left, depth);
    fprintf(stderr, ", r=");
    print_ast_sub(nn->right, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == AssignExpr) {
    shared_ptr<ASTAssignExpr> nn = dynamic_pointer_cast<ASTAssignExpr>(n);
    fprintf(stderr, "AssignExpr(l=");
    print_ast_sub(nn->left, depth);
    fprintf(stderr, ", r=");
    print_ast_sub(nn->right, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == ExprStmt) {
    shared_ptr<ASTExprStmt> nn = dynamic_pointer_cast<ASTExprStmt>(n);
    fprintf(stderr, "ExprStmt(expr=");
    print_ast_sub(nn->expr, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == BreakStmt) {
    shared_ptr<ASTBreakStmt> nn = dynamic_pointer_cast<ASTBreakStmt>(n);
    fprintf(stderr, "BreakStmt");
    return;
  }
  if (n->type == ContinueStmt) {
    shared_ptr<ASTContinueStmt> nn = dynamic_pointer_cast<ASTContinueStmt>(n);
    fprintf(stderr, "ContinueStmt");
    return;
  }
  if (n->type == ReturnStmt) {
    shared_ptr<ASTReturnStmt> nn = dynamic_pointer_cast<ASTReturnStmt>(n);
    fprintf(stderr, "ReturnStmt(expr=");
    print_ast_sub(nn->expr, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == Declarator) {
    shared_ptr<ASTDeclarator> nn = dynamic_pointer_cast<ASTDeclarator>(n);
    fprintf(stderr, "Declarator<%.*s>", nn->op->length, nn->op->begin);
    return;
  }
  if (n->type == Declaration) {
    shared_ptr<ASTDeclaration> nn = dynamic_pointer_cast<ASTDeclaration>(n);
    fprintf(stderr, "Declaration(ds=");
    print_ast_sub(nn->declaration_spec, depth);
    fprintf(stderr, ", d-list=");
    print_ast_vec(nn->declarators, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == SimpleDeclaration) {
    shared_ptr<ASTSimpleDeclaration> nn = dynamic_pointer_cast<ASTSimpleDeclaration>(n);
    fprintf(stderr, "SimpleDeclaration(ts=");
    print_ast_sub(nn->type_spec, depth);
    fprintf(stderr, ", d=");
    print_ast_sub(nn->declarator, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == CompoundStmt) {
    shared_ptr<ASTCompoundStmt> nn = dynamic_pointer_cast<ASTCompoundStmt>(n);
    fprintf(stderr, "CompoundStmt(item-list=");
    print_ast_vec(nn->items, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == FuncDeclarator) {
    shared_ptr<ASTFuncDeclarator> nn = dynamic_pointer_cast<ASTFuncDeclarator>(n);
    fprintf(stderr, "FuncDeclarator(d=");
    print_ast_sub(nn->declarator, depth);
    fprintf(stderr, ", args=");
    print_ast_vec(nn->args, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == FuncDeclaration) {
    shared_ptr<ASTFuncDeclaration> nn = dynamic_pointer_cast<ASTFuncDeclaration>(n);
    fprintf(stderr, "FuncDeclaration(ts=");
    print_ast_sub(nn->type_spec, depth);
    fprintf(stderr, ", d=");
    print_ast_sub(nn->declarator, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == FuncDef) {
    shared_ptr<ASTFuncDef> nn = dynamic_pointer_cast<ASTFuncDef>(n);
    fprintf(stderr, "FuncDef(d=");
    print_ast_sub(nn->declaration, depth);
    fprintf(stderr, ", body=");
    print_ast_sub(nn->body, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == ExternalDeclaration) {
    shared_ptr<ASTExternalDeclaration> nn = dynamic_pointer_cast<ASTExternalDeclaration>(n);
    fprintf(stderr, "ExternalDeclaration(ds=");
    print_ast_sub(nn->declaration_spec, depth);
    fprintf(stderr, ", d-list=");
    print_ast_vec(nn->declarators, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == TranslationUnit) {
    shared_ptr<ASTTranslationUnit> nn = dynamic_pointer_cast<ASTTranslationUnit>(n);
    fprintf(stderr, "TranslationUnit(ed-list=");
    print_ast_vec(nn->external_declarations, depth);
    fprintf(stderr, ")");
    return;
  }
  assert(false);
}

void print_ast(shared_ptr<AST> n) {
  print_ast_sub(n, 0);
  fputc('\n', stderr);
}

shared_ptr<ASTTranslationUnit> parse(Token **head_token, Error &err) {
  init_parser(head_token);
  Token *next = *head_token;
  return parse_translation_unit(&next, err);
}