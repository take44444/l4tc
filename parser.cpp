#include "lupc.h"

ASTFuncDeclaratorNode *parse_func_declarator(TokenNode **next, Error &err) {
// ident(type ident, ...)
  TokenNode *t = expect_token_with_type(next, err, Ident);
  if (t == NULL) return NULL;
  ASTFuncDeclaratorNode *ret = new ASTFuncDeclaratorNode(t);
  t = expect_token_with_str(next, err, "(");
  if (t == NULL) return NULL;
  // TODO: parse args
  // ret->left =
  t = expect_token_with_str(next, err, ")");
  if (t == NULL) return NULL;
  return ret;
}

ASTFuncDeclarationNode *parse_func_declaration(TokenNode **next, Error &err) {
// func-declarator -> type
  ASTNode *left = parse_func_declarator(next, err);
  if (left == NULL) return NULL;
  TokenNode *t = expect_token_with_str(next, err, "->");
  if (t == NULL) return NULL;
  t = consume_token_with_type(next, KwNum);
  if (t == NULL) consume_token_with_type(next, KwStr);
  if (t == NULL) {
    err = Error("");
    return NULL;
  }
  ASTFuncDeclarationNode *ret = new ASTFuncDeclarationNode(t);
  ret->left = left;
  return ret;
}

ASTFuncDefNode *parse_func_def(TokenNode **next, Error &error) {
// func-declaration compound-stmt
  TokenNode *t = expect_token_with_type(next, error, KwFunc);
  if (t == NULL) return NULL;
  ASTFuncDefNode *ret = new ASTFuncDefNode(t);
  ret->left = parse_func_declaration(next, error);
  if (t == NULL) return NULL;
  // ret->right = parse_comp_stmt(next, error);
  if (t == NULL) return NULL;
  return ret;
}

ASTNode *parse_external_declaration(TokenNode **next, Error &err) {
  if ((*next)->is_equal_with_str("func")) return parse_func_def(next, err);
  else return NULL;
  // else parse_declaration(next);
}

ASTNode *parse(TokenNode *head_token) {
  TokenNode *next = head_token;
  Error error = Error("");
  ASTNode *head = parse_external_declaration(&next, error);
  for (
    ASTNode *external_declaration = head;
    external_declaration != NULL;
    external_declaration = parse_external_declaration(&next, error)
  );
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