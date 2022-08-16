#include "bits/stdc++.h"

using namespace std;

class Error {
  private:
  string err_str;
  public:
  Error(string err) {
    err_str = err;
  }
};

enum TokenType {
  Delimiter,      // space
  Punctuator,     // , ( ) + <- -> = > & * | && || \n
  // 数値
  NumberConstant, // 0 3 0x07 0b10
  // 文字列リテラル
  StringLiteral,  // "hello world"
  // 変数
  Ident,          // vAr_n4m3
  // 予約語
  KwBreak,        // break
  KwContinue,     // continue
  KwElif,         // elif
  KwElse,         // else
  KwFor,          // for
  KwFunc,         // func
  KwIf,           // if
  KwLoop,         // loop
  KwNum,          // num
  KwReturn,       // return
  KwStr,          // str
  KwVoid,         // void
  // 予期しない文字
  Unknown,        // unknown
};

class TokenNode {
  public:
  enum TokenType type;
  TokenNode *next;
  char *begin;
  int length;
  int line;
  TokenNode(int l, char *beg, int len, enum TokenType tp) {
    line = l;
    begin = beg;
    length = len;
    type = tp;
    next = NULL;
  }

  bool is_equal_with_str(const char *s) {
    return strlen(s) == (size_t)length && strncmp(begin, s, length) == 0;
  }

  void print() {
    fprintf(stdout, "%.*s", length, begin);
  }
};

// utils.cpp
TokenNode *expect_token_with_str(TokenNode **next, Error &err, string str);
TokenNode *consume_token_with_str(TokenNode **next, string str);
TokenNode *expect_token_with_type(TokenNode **next, Error &err, TokenType type);
TokenNode *consume_token_with_type(TokenNode **next, TokenType type);

// tokenizer.cpp
TokenNode *create_next_token(char *p, int &line);
void print_tokens(TokenNode *head);
TokenNode *tokenize(string &source);

class ASTNode {
  public:
  TokenNode *op;
  ASTNode(TokenNode *t) {
    op = t;
  }
};

class ASTFuncDefNode : public ASTNode {
  public:
  ASTNode *left;
  ASTNode *right;
  ASTFuncDefNode(TokenNode *t) : ASTNode(t) {}
};

class ASTFuncDeclarationNode : public ASTNode {
  public:
  ASTNode *left;
  ASTFuncDeclarationNode(TokenNode *t) : ASTNode(t) {}
};

class ASTFuncDeclaratorNode : public ASTNode {
  public:
  ASTFuncDeclaratorNode(TokenNode *t) : ASTNode(t) {}
};

// parser.cpp
ASTFuncDeclaratorNode *parse_func_declarator(TokenNode **next, Error &err);
ASTFuncDeclarationNode *parse_func_declaration(TokenNode **next, Error &err);
ASTFuncDefNode *parse_func_def(TokenNode **next, Error &error);
ASTNode *parse_external_declaration(TokenNode **next, Error &err);
ASTNode *parse(TokenNode *head);