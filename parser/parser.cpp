#include "./parser.hpp"

namespace parser {
  std::shared_ptr<ASTTypeSpec> parse_type_spec(tokenizer::Token **next, Error &err) {
    tokenizer::Token *t;
    if (
      (t = consume_token_with_type(next, tokenizer::KwNum)) ||
      (t = consume_token_with_type(next, tokenizer::KwStr)) ||
      (t = consume_token_with_type(next, tokenizer::KwVoid))
    ) {
      return std::make_shared<ASTTypeSpec>(t);
    }
    err = Error("expected type-specifier, found ????");
    return nullptr;
  }

  std::shared_ptr<ASTTypeSpec> parse_declaration_spec(tokenizer::Token **next, Error &err) {
    return parse_type_spec(next, err);
  }

  std::shared_ptr<ASTExpr> parse_expr(tokenizer::Token **next, Error &err);
  std::shared_ptr<ASTExpr> parse_primary_expr(tokenizer::Token **next, Error &err) {
    tokenizer::Token *t;
    if (
      (t = consume_token_with_type(next, tokenizer::NumberConstant)) ||
      (t = consume_token_with_type(next, tokenizer::Ident))
    ) {
      return std::make_shared<ASTSimpleExpr>(t);
    }
    if (consume_token_with_str(next, "(")) {
      std::shared_ptr<ASTPrimaryExpr> ret = std::make_shared<ASTPrimaryExpr>();
      if (!(ret->expr = parse_expr(next, err))) return nullptr;
      if (expect_token_with_str(next, err, ")")) return ret;
    }
    err = Error("expected primary-expr, found ????");
    return nullptr;
  }

  std::shared_ptr<ASTExpr> parse_assign_expr(tokenizer::Token **next, Error &err);
  std::shared_ptr<ASTExpr> parse_postfix_expr(tokenizer::Token **next, Error &err) {
  // function-call
  // array
  // increment
  // decrement
  // ->
  // .
    std::shared_ptr<ASTExpr> primary = parse_primary_expr(next, err);
    if (!primary) return nullptr;
    // if "(" is not here, it is not function-call
    // but it is correct primary expr
    if (!consume_token_with_str(next, "(")) return primary;
    // now it is assignment-expr
    std::shared_ptr<ASTFuncCallExpr> ret = std::make_shared<ASTFuncCallExpr>();
    ret->primary = primary;

    std::shared_ptr<ASTExpr> arg;
    while (!consume_token_with_str(next, ")")) {
      if (!(arg = parse_assign_expr(next, err))) return nullptr;
      ret->args.push_back(arg);
      if (consume_token_with_str(next, ",")) continue;
      // end
      if (!expect_token_with_str(next, err, ")")) return nullptr;
      break;
    }
    return ret;
  }

  std::shared_ptr<ASTExpr> parse_unary_expr(tokenizer::Token **next, Error &err) {
    return parse_postfix_expr(next, err);
  }

  std::shared_ptr<ASTExpr> parse_multiplicative_expr(tokenizer::Token **next, Error &err) {
    std::shared_ptr<ASTExpr> left, ret = parse_unary_expr(next, err);
    if (!ret) return nullptr;
    while (
      consume_token_with_str(next, "*") ||
      consume_token_with_str(next, "/") ||
      consume_token_with_str(next, "%")
    ) {
      left = ret;
      ret = std::make_shared<ASTMultiplicativeExpr>();
      ret->left = left;
      if (!(ret->right = parse_unary_expr(next, err))) return nullptr;
    }
    return ret;
  }

  std::shared_ptr<ASTExpr> parse_additive_expr(tokenizer::Token **next, Error &err) {
    std::shared_ptr<ASTExpr> left, ret = parse_multiplicative_expr(next, err);
    if (!ret) return nullptr;
    while (
      consume_token_with_str(next, "+") ||
      consume_token_with_str(next, "-")
    ) {
      left = ret;
      ret = std::make_shared<ASTAdditiveExpr>();
      ret->left = left;
      if (!(ret->right = parse_multiplicative_expr(next, err))) return nullptr;
    }
    return ret;
  }

  std::shared_ptr<ASTExpr> parse_shift_expr(tokenizer::Token **next, Error &err) {
    std::shared_ptr<ASTExpr> left, ret = parse_additive_expr(next, err);
    if (!ret) return nullptr;
    while (
      consume_token_with_str(next, "<<") ||
      consume_token_with_str(next, ">>")
    ) {
      left = ret;
      ret = std::make_shared<ASTShiftExpr>();
      ret->left = left;
      if (!(ret->right = parse_additive_expr(next, err))) return nullptr;
    }
    return ret;
  }

  std::shared_ptr<ASTExpr> parse_relational_expr(tokenizer::Token **next, Error &err) {
    std::shared_ptr<ASTExpr> left, ret = parse_shift_expr(next, err);
    if (!ret) return nullptr;
    while (
      consume_token_with_str(next, "<") ||
      consume_token_with_str(next, ">") ||
      consume_token_with_str(next, "<=") ||
      consume_token_with_str(next, ">=")
    ) {
      left = ret;
      ret = std::make_shared<ASTRelationalExpr>();
      ret->left = left;
      if (!(ret->right = parse_shift_expr(next, err))) return nullptr;
    }
    return ret;
  }

  std::shared_ptr<ASTExpr> parse_equality_expr(tokenizer::Token **next, Error &err) {
    std::shared_ptr<ASTExpr> left, ret = parse_relational_expr(next, err);
    if (!ret) return nullptr;
    while (
      consume_token_with_str(next, "==") ||
      consume_token_with_str(next, "!=")
    ) {
      left = ret;
      ret = std::make_shared<ASTEqualityExpr>();
      ret->left = left;
      if (!(ret->right = parse_relational_expr(next, err))) return nullptr;
    }
    return ret;
  }

  std::shared_ptr<ASTExpr> parse_bitwise_and_expr(tokenizer::Token **next, Error &err) {
    std::shared_ptr<ASTExpr> left, ret = parse_equality_expr(next, err);
    if (!ret) return nullptr;
    while (consume_token_with_str(next, "^")) {
      left = ret;
      ret = std::make_shared<ASTBitwiseAndExpr>();
      ret->left = left;
      if (!(ret->right = parse_equality_expr(next, err))) return nullptr;
    }
    return ret;
  }

  std::shared_ptr<ASTExpr> parse_bitwise_xor_expr(tokenizer::Token **next, Error &err) {
    std::shared_ptr<ASTExpr> left, ret = parse_bitwise_and_expr(next, err);
    if (!ret) return nullptr;
    while (consume_token_with_str(next, "^")) {
      left = ret;
      ret = std::make_shared<ASTBitwiseXorExpr>();
      ret->left = left;
      if (!(ret->right = parse_bitwise_and_expr(next, err))) return nullptr;
    }
    return ret;
  }

  std::shared_ptr<ASTExpr> parse_bitwise_or_expr(tokenizer::Token **next, Error &err) {
    std::shared_ptr<ASTExpr> left, ret = parse_bitwise_xor_expr(next, err);
    if (!ret) return nullptr;
    while (consume_token_with_str(next, "|")) {
      left = ret;
      ret = std::make_shared<ASTBitwiseOrExpr>();
      ret->left = left;
      if (!(ret->right = parse_bitwise_xor_expr(next, err))) return nullptr;
    }
    return ret;
  }

  std::shared_ptr<ASTExpr> parse_logical_and_expr(tokenizer::Token **next, Error &err) {
    std::shared_ptr<ASTExpr> left, ret = parse_bitwise_or_expr(next, err);
    if (!ret) return nullptr;
    while (consume_token_with_str(next, "&&")) {
      left = ret;
      ret = std::make_shared<ASTLogicalAndExpr>();
      ret->left = left;
      if (!(ret->right = parse_bitwise_or_expr(next, err))) return nullptr;
    }
    return ret;
  }

  std::shared_ptr<ASTExpr> parse_logical_or_expr(tokenizer::Token **next, Error &err) {
    std::shared_ptr<ASTExpr> left, ret = parse_logical_and_expr(next, err);
    if (!ret) return nullptr;
    while (consume_token_with_str(next, "||")) {
      left = ret;
      ret = std::make_shared<ASTLogicalOrExpr>();
      ret->left = left;
      if (!(ret->right = parse_logical_and_expr(next, err))) return nullptr;
    }
    return ret;
  }

  std::shared_ptr<ASTExpr> parse_assign_expr(tokenizer::Token **next, Error &err) {
    std::shared_ptr<ASTExpr> left = parse_logical_or_expr(next, err);
    // parse error
    if (!left) return nullptr;
    // if it is not unary-expr, it can't be left of assignment-expr
    // but it is correct expr
    if (!is_unary_expr(left)) return left;
    // if ":" is not here, it is not assignment-expr
    // but it is correct expr
    if (!consume_token_with_str(next, ":")) return left;
    // now it is assignment-expr
    std::shared_ptr<ASTAssignExpr> ret = std::make_shared<ASTAssignExpr>();
    ret->left = left;
    if (!(ret->right = parse_assign_expr(next, err))) return nullptr;
    return ret;
  }

  std::shared_ptr<ASTExpr> parse_expr(tokenizer::Token **next, Error &err) {
    return parse_assign_expr(next, err);
  }

  std::shared_ptr<ASTExprStmt> parse_expr_stmt(tokenizer::Token **next, Error &err) {
    std::shared_ptr<ASTExprStmt> ret = std::make_shared<ASTExprStmt>();
    if (!(ret->expr = parse_expr(next, err))) return nullptr;
    if (!expect_token_with_str(next, err, "\n")) return nullptr;
    return ret;
  }

  std::shared_ptr<ASTDeclarator> parse_declarator(tokenizer::Token **next, Error &err) {
    tokenizer::Token *t = consume_token_with_type(next, tokenizer::Ident);
    if (!t) {
      err = Error("expected declarator, found ????");
      return nullptr;
    }
    return std::make_shared<ASTDeclarator>(t);
  }

  std::shared_ptr<ASTDeclaration> parse_declaration(tokenizer::Token **next, Error &err) {
    std::shared_ptr<ASTDeclaration> ret = std::make_shared<ASTDeclaration>();
    if (!(ret->declaration_spec = parse_declaration_spec(next, err))) return nullptr;

    std::shared_ptr<ASTDeclarator> declarator;
    while ((declarator = parse_declarator(next, err))) {
      ret->declarators.push_back(declarator);
      if (consume_token_with_str(next, ",")) continue;
      // declaration end
      if (expect_token_with_str(next, err, "\n")) return ret;
      break;
    }
    err = Error("expected declarator, found ????");
    return nullptr;
  }

  std::shared_ptr<ASTSimpleDeclaration> parse_simple_declaration(tokenizer::Token **next, Error &err) {
  // type-specifier declarator
    std::shared_ptr<ASTSimpleDeclaration> ret = std::make_shared<ASTSimpleDeclaration>();
    if (!(ret->type_spec = parse_type_spec(next, err))) return nullptr;
    if (!(ret->declarator = parse_declarator(next, err))) return nullptr;
    return ret;
  }

  std::shared_ptr<ASTCompoundStmt> parse_comp_stmt(tokenizer::Token **next, Error &err, int indents);
  std::shared_ptr<AST> parse_stmt(tokenizer::Token **next, Error &err) {
    if (consume_token_with_type(next, tokenizer::KwBreak)) {
      if (!expect_token_with_str(next, err, "\n")) return nullptr;
      return std::make_shared<ASTBreakStmt>();
    }
    if (consume_token_with_type(next, tokenizer::KwContinue)) {
      if (!expect_token_with_str(next, err, "\n")) return nullptr;
      return std::make_shared<ASTContinueStmt>();
    }
    if (consume_token_with_type(next, tokenizer::KwReturn)) {
      std::shared_ptr<ASTReturnStmt> ret = std::make_shared<ASTReturnStmt>();
      if (!(ret->expr = parse_expr(next, err))) return nullptr;
      if (!expect_token_with_str(next, err, "\n")) return nullptr;
      return ret;
    }
    return parse_expr_stmt(next, err);
    // TODO: if, loop
  }

  std::shared_ptr<ASTCompoundStmt> parse_comp_stmt(tokenizer::Token **next, Error &err, int indents) {
    std::shared_ptr<ASTCompoundStmt> ret = std::make_shared<ASTCompoundStmt>();
    std::shared_ptr<AST> item;
    while (1) {
      if (!consume_token_with_indents(next, indents)) {
        if (!*next || (*next)->sv.front() != ' ' || (int)(*next)->sv.length() < indents) {
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

  std::shared_ptr<ASTFuncDeclarator> parse_func_declarator(tokenizer::Token **next, Error &err) {
  // declarator(declaration, ...)
    std::shared_ptr<ASTFuncDeclarator> ret = std::make_shared<ASTFuncDeclarator>();
    if (!(ret->declarator = parse_declarator(next, err))) return nullptr;
    if (!expect_token_with_str(next, err, "(")) return nullptr;

    std::shared_ptr<ASTSimpleDeclaration> declaration;
    while (!consume_token_with_str(next, ")")) {
      if (!(declaration = parse_simple_declaration(next, err))) return nullptr;
      ret->args.push_back(declaration);
      if (consume_token_with_str(next, ",")) continue;
      // end
      if (!expect_token_with_str(next, err, ")")) return nullptr;
      break;
    }
    return ret;
  }

  std::shared_ptr<ASTFuncDeclaration> parse_func_declaration(tokenizer::Token **next, Error &err) {
  // func-declarator -> type
    std::shared_ptr<ASTFuncDeclaration> ret = std::make_shared<ASTFuncDeclaration>();
    if (!(ret->declarator = parse_func_declarator(next, err))) return nullptr;
    // token "->" should be here
    if (!expect_token_with_str(next, err, "->")) return nullptr;
    if (!(ret->type_spec = parse_type_spec(next, err))) return nullptr;
    // token LF should be here
    if (!expect_token_with_str(next, err, "\n")) return nullptr;
    return ret;
  }

  std::shared_ptr<ASTFuncDef> parse_func_def(tokenizer::Token **next, Error &err) {
  // func-declaration compound-stmt
    if (!expect_token_with_type(next, err, tokenizer::KwFunc)) return nullptr;
    std::shared_ptr<ASTFuncDef> ret = std::make_shared<ASTFuncDef>();
    if (!(ret->declaration = parse_func_declaration(next, err))) return nullptr;
    if (!(ret->body = parse_comp_stmt(next, err, 2))) return nullptr;
    return ret;
  }

  std::shared_ptr<ASTExternalDeclaration> parse_external_declaration(tokenizer::Token **next, Error &err) {
    std::shared_ptr<ASTExternalDeclaration> ret = std::make_shared<ASTExternalDeclaration>();
    if (!(ret->declaration_spec = parse_declaration_spec(next, err))) return nullptr;

    std::shared_ptr<ASTDeclarator> declarator;
    while ((declarator = parse_declarator(next, err))) {
      ret->declarators.push_back(declarator);
      if (consume_token_with_str(next, ",")) continue;
      // declaration end
      if (expect_token_with_str(next, err, "\n")) return ret;
      break;
    }
    err = Error("expected declarator, found ????");
    return nullptr;
  }

  std::shared_ptr<ASTTranslationUnit> parse_translation_unit(tokenizer::Token **next, Error &err) {
    std::shared_ptr<ASTTranslationUnit> ret = std::make_shared<ASTTranslationUnit>();

    std::shared_ptr<AST> external_declaration;
    while (*next) {
      if (
        !(external_declaration = parse_func_def(next, err)) &&
        !(external_declaration = parse_external_declaration(next, err))
      ) return nullptr;
      ret->external_declarations.push_back(external_declaration);
    }
    return ret;
  }

  void init_parser(tokenizer::Token **head_token) {
    // *head_token can be NULL
    tokenizer::Token *next = *head_token;
    tokenizer::Token *t, *bef;
    while (next) {
      // if next token is Delimiter, remove it and continue
      t = consume_token_with_type(&next, tokenizer::Delimiter);    
      if (!t) break;
      delete t;
    }
    // set new head_token
    *head_token = next;
    // remove first LF punctuator
    if (*head_token) {
      t = consume_token_with_str(head_token, "\n");
      if (!t) delete t;
    }
    // *head_token can be NULL
    next = *head_token;
    while (next) {
      bef = next;
      assert(bef->type != tokenizer::Delimiter);
      next = bef->next;
      while (next) {
        // if next token is Delimiter, remove it and continue
        t = consume_token_with_type(&next, tokenizer::Delimiter);
        if (!t) break;
        delete t;
      }
      bef->next = next;
    }
  }

  std::shared_ptr<ASTTranslationUnit> parse(tokenizer::Token **head_token, Error &err) {
    init_parser(head_token);
    tokenizer::Token *next = *head_token;
    return parse_translation_unit(&next, err);
  }
}