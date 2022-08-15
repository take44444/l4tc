#include "lupc.h"

ASTNode *parse_func_def(TokenNode **next, Error &error) {
// func-declaration compound-stmt
  TokenNode *t = expect_token_with_type(next, error, KwFunc);
  if (t == NULL) return NULL;
  ASTNode *ret = new ASTFuncDefNode(t);
  ret->left = parse_func_declaration(next, error);
  if (t == NULL) return NULL;
  ret->right = parse_comp_stmt(next, error);
  if (t == NULL) return NULL;
  return ret;
}

ASTNode *parse_func_declaration(TokenNode **next, Error &error) {
// func-declarator -> type
  ASTNode *left = parse_func_declarator(next, error);
  if (left == NULL) return NULL;
  TokenNode *t = expect_token_with_str(next, error, "->");
  if (t == NULL) return NULL;
  t = consume_token_with_type(next, KwNum);
  if (t == NULL) consume_token_with_type(next, KwStr);
  if (t == NULL) {
    error = Error();
    return NULL;
  }
  ASTNode *ret = new ASTFuncDeclarationNode(t);
  ret->left = left;
  return ret;
}

ASTNode *parse_func_declarator(TokenNode **next, Error &error) {
// ident(type ident, ...)
  TokenNode *t = expect_token_with_type(next, error, Ident);
  if (t == NULL) return NULL;
  ASTNode *ret = new ASTFuncDeclaratorNode(t);
  t = expect_token_with_str(next, error, '(');
  if (t == NULL) return NULL;
  // TODO: parse args
  // ret->left =
  t = expect_token_with_str(next, error, ')');
  if (t == NULL) return NULL;
  return ret;
}

ASTNode *parse_external_declaration(TokenNode **next) {
  if ((*next)->is_equal_with_str("func")) parse_func_def(next);
  else parse_declaration(next);
}

ASTNode *parse(TokenNode *head) {
  TokenNode *next = head;
  for (
    ASTNode *external_declaration = parse_external_declaration(&next);
    external_declaration != NULL;
    external_declaration = parse_external_declaration(&next)
  );
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