#include "lupc.h"

shared_ptr<ASTTypeSpec> parse_type_spec(Token **next, Error &err) {
  // NULL check
  if (!*next) {
    err = Error("expected type-specifier, found EOF");
    return NULL;
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
    return NULL;
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
    return NULL;
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
    return NULL;
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
    return NULL;
  }
  shared_ptr<ASTCompoundStmt> ret = make_shared<ASTCompoundStmt>();
  shared_ptr<AST> item;
  while (1) {
    if (!consume_token_with_indents(next, indents)) {
      if (*((*next)->begin) != ' ' || (*next)->length < indents) {
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
  while (!!next) { // NULL check
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
  while (next != NULL) {
    // if next token is Delimiter, remove it and continue
    t = consume_token_with_type(&next, Delimiter);    
    if (!t) break;
    delete t;
  }
  // set new head_token
  *head_token = next;
  // remove first LF punctuator
  if (*head_token != NULL) {
    t = consume_token_with_str(head_token, "\n");
    if (t != NULL) delete t;
  }
  // *head_token can be NULL
  next = *head_token;
  while (next != NULL) {
    bef = next;
    assert(bef->type != Delimiter);
    next = bef->next;
    while (next != NULL) {
      // if next token is Delimiter, remove it and continue
      t = consume_token_with_type(&next, Delimiter);
      if (!t) break;
      delete t;
    }
    bef->next = next;
  }
}

shared_ptr<ASTTranslationUnit> parse(Token **head_token, Error &err) {
  init_parser(head_token);
  Token *next = *head_token;
  return parse_translation_unit(&next, err);
}