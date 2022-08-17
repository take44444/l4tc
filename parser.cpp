#include "lupc.h"

ASTExprNode *parse_expr(TokenNode **next, Error &err) {

}

ASTExprStmtNode *parse_expr_stmt(TokenNode **next, Error &err) {
  // parse expr
  ASTExprNode *expr = parse_expr(next, err);
  // check parse error
  if (expr == NULL) return NULL;
  // token "\n" should be here
  if (!check_lf(next, err)) return NULL;
  ASTExprStmtNode *ret = new ASTExprStmtNode(*next);
  ret->expr = expr;
  return ret;
}

ASTReturnStmtNode *parse_return_stmt(TokenNode **next, Error &err) {
  // token KwReturn should be here
  TokenNode *t = expect_token_with_type(next, err, KwReturn);
  if (t == NULL) return NULL;
  ASTReturnStmtNode *ret = new ASTReturnStmtNode(t);
  // parse expr
  ASTExprNode *expr = parse_expr(next, err);
  // check parse error
  if (expr == NULL) return NULL;
  ret->expr = expr;
  return ret;
}

ASTDeclaratorNode *parse_declarator(TokenNode **next, Error &err) {
  // token Ident should be here
  TokenNode *t = expect_token_with_type(next, err, Ident);
  if (t == NULL) return NULL;
  return new ASTDeclaratorNode(t);
}

ASTDeclarationNode *parse_declaration(TokenNode **next, Error &err) {
  // parse type-specifer
  TokenNode *t = consume_token_with_type(next, KwNum);
  if (t == NULL) t = consume_token_with_type(next, KwStr);
  // check parse error
  if (t == NULL) {
    err = Error("expected type-specifier");
    return NULL;
  }
  ASTDeclarationNode *ret = new ASTDeclarationNode(t);
  ret->declarators = vector<ASTDeclaratorNode *>();
  while (1) {
    // parse declarator
    ASTDeclaratorNode *declarator = parse_declarator(next, err);
    // check parse error
    if (declarator == NULL) return NULL;
    ret->declarators.push_back(declarator);
    // declaration end
    if (*((*next)->begin) == '\n') break;
    // token "," should be here
    t = expect_token_with_str(next, err, ",");
    if (t == NULL) return NULL;
  }
  return ret;
}

ASTDeclarationNode *parse_single_declaration(TokenNode **next, Error &err) {
  // parse type-specifer
  TokenNode *t = consume_token_with_type(next, KwNum);
  if (t == NULL) t = consume_token_with_type(next, KwStr);
  // check parse error
  if (t == NULL) {
    err = Error("expected type-specifier");
    return NULL;
  }
  ASTDeclarationNode *ret = new ASTDeclarationNode(t);
  ret->declarators = vector<ASTDeclaratorNode *>();
  // parse declarator
  ASTDeclaratorNode *declarator = parse_declarator(next, err);
  // check parse error
  if (declarator == NULL) return NULL;
  ret->declarators.push_back(declarator);
  return ret;
}

ASTCompoundStmtNode *parse_comp_stmt(TokenNode **next, Error &err, int indents);
ASTOtherStmtNode *parse_other_stmt(TokenNode **next, Error &err) {
  ASTOtherStmtNode *ret;
  switch ((*next)->type)
  {
  case KwBreak:
    ret = new ASTBreakStmtNode(consume_token_with_type(next, KwBreak));
    break;
  case KwContinue:
    ret = new ASTContinueStmtNode(consume_token_with_type(next, KwContinue));
    break;
  case KwReturn:
    ret = parse_return_stmt(next, err);
    break;
  // case KwIf:
  //   ret = parse_selection_stmt(next, err);
  //   break;
  // case KwLoop:
  //   ret = parse_loop_stmt(next, err);
  //   break;
  default:
    ret = parse_expr_stmt(next, err);
    break;
  }
  return ret;
}

ASTCompoundStmtNode *parse_comp_stmt(TokenNode **next, Error &err, int indents) {
  if (!check_lf(next, err)) return NULL;
  ASTCompoundStmtNode *ret = new ASTCompoundStmtNode(*next);
  while (1) {
    // token "\n" should be here
    if (!check_lf(next, err)) return NULL;
    if (consume_token_with_indents(next, indents) == NULL) {
      if ((*next)->length > indents) {
      // inner compound-stmt
        // parse compound-stmt
        ASTCompoundStmtNode *comp_stmt = parse_comp_stmt(next, err, indents + 2);
        // check parse error
        if (comp_stmt == NULL) return NULL;
        ret->stmts.push_back(comp_stmt);
      } else {
      // compound-stmt end
        break;
      }
    } else if ((*next)->type == KwNum || (*next)->type == KwStr) {
    // declaration
      // parse declaration
      ASTDeclarationNode *declaration = parse_declaration(next, err);
      // check parse error
      if (declaration == NULL) return NULL;
      ret->stmts.push_back(declaration);
    } else {
    // other-stmt
      // parse other-stmt
      ASTOtherStmtNode *other_stmt = parse_other_stmt(next, err);
      // check parse error
      if (other_stmt == NULL) return NULL;
      ret->stmts.push_back(other_stmt);
    }
  }
  return ret;
}

ASTFuncDeclaratorNode *parse_func_declarator(TokenNode **next, Error &err) {
// ident(type ident, ...)
  // token Ident should be here
  TokenNode *t = expect_token_with_type(next, err, Ident);
  if (t == NULL) return NULL;
  ASTFuncDeclaratorNode *ret = new ASTFuncDeclaratorNode(t);
  // token "(" should be here
  if (expect_token_with_str(next, err, "(") == NULL) return NULL;
  while (!(*next)->is_equal_with_str(")")) {
    // parse single-declaration
    ASTDeclarationNode *declaration = parse_single_declaration(next, err);
    // check parse error
    if (declaration == NULL) return NULL;
    ret->args.push_back(declaration);
    // args end
    if (consume_token_with_str(next, ",") == NULL) break;
  }
  // token ")" should be here
  if (expect_token_with_str(next, err, ")") == NULL) return NULL;
  return ret;
}

ASTFuncDeclarationNode *parse_func_declaration(TokenNode **next, Error &err) {
// func-declarator -> type
  // parse function-declarator
  ASTFuncDeclaratorNode *declarator = parse_func_declarator(next, err);
  // check parse error
  if (declarator == NULL) return NULL;
  // token "->" should be here
  if (expect_token_with_str(next, err, "->") == NULL) return NULL;
  // parse type-specifer
  TokenNode *t = consume_token_with_type(next, KwNum);
  if (t == NULL) t = consume_token_with_type(next, KwStr);
  if (t == NULL) t = consume_token_with_type(next, KwVoid);
  // check parse error
  if (t == NULL) {
    err = Error("expected type-specifier");
    return NULL;
  }
  ASTFuncDeclarationNode *ret = new ASTFuncDeclarationNode(t);
  ret->declarator = declarator;
  return ret;
}

ASTFuncDefNode *parse_func_def(TokenNode **next, Error &err) {
// func-declaration compound-stmt
  // NULL check
  if ((*next) == NULL) {
    err = Error("expected function-definition but found EOF");
    return NULL;
  }
  // token KwFunc should be here
  TokenNode *t = expect_token_with_type(next, err, KwFunc);
  if (t == NULL) return NULL;
  ASTFuncDefNode *ret = new ASTFuncDefNode(t);
  // parse function-declaration
  ASTFuncDeclarationNode *func_declaration = parse_func_declaration(next, err);
  // check parse error
  if (func_declaration == NULL) return NULL;
  ret->declaration = func_declaration;
  // parse compound-stmt
  ASTCompoundStmtNode *comp_stmt = parse_comp_stmt(next, err, 2);
  // check parse error
  if (comp_stmt == NULL) return NULL;
  ret->body = comp_stmt;
  return ret;
}

ASTNode *parse_external_declaration(TokenNode **next, Error &err) {
  // NULL check
  if ((*next) == NULL) {
    err = Error("expected external-declaration but found EOF");
    return NULL;
  }
  ASTNode *ret;
  if ((*next)->is_equal_with_str("func")) ret = parse_func_def(next, err);
  else ret = parse_declaration(next, err);
  // check parse error
  if (ret == NULL) return NULL;

  // need LF + 0 indents after external-declaration
  if (check_lf(next, err)) {
    if (expect_token_with_indents(next, err, 0) == NULL) return NULL;
  } else return NULL;
  return ret;
}

ASTNode *parse(TokenNode *head_token, Error &err) {
  TokenNode *next = head_token;
  // head AST node
  ASTNode *head = parse_external_declaration(&next, err);
  for (
    ASTNode *external_declaration = head;;
    external_declaration = parse_external_declaration(&next, err)
  ) {
    // check parse error
    if (external_declaration == NULL) return NULL;
    // if it was last node, finish
    if (next == NULL) break;
  }
  return head;
  // external declaration
  // static num a: 3, v: 3;  : declaration
  // const num a: 4;         : declaration
  // num a. b                : declaration
  // 
  // func f(x) -> num        : function-definition
  //   a = 3
  //   num c
  // 
  // struct name
  //   num a
}