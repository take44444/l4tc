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
bool check_lf(TokenNode **next, Error &err);
TokenNode *consume_token_with_indents(TokenNode **next, int indents);
TokenNode *expect_token_with_indents(TokenNode **next, Error &err, int indents);

// tokenizer.cpp
TokenNode *create_next_token(char *p, int &line);
void print_tokens(TokenNode *head);
TokenNode *tokenize(string &source);

enum ASTType {
  Declaration,
  Declarator,
  FuncDef,
  FuncDeclaration,
  FuncDeclarator,
  BreakStmt,
  ContinueStmt,
  ReturnStmt,
  CompoundStmt,
  ExprStmt,
  SelectionStmt,
  LoopStmt,
};

class ASTNode {
  public:
  TokenNode *op;
  ASTType type;
  ASTNode(TokenNode *t, ASTType tp) : op(t), type(tp) {}
};

class ASTCompoundStmtNode : public ASTNode {
  public:
  vector<ASTNode *> stmts;
  ASTCompoundStmtNode(TokenNode *t) : ASTNode(t, CompoundStmt) {
    stmts = vector<ASTNode *>();
  }
};

class ASTDeclaratorNode : public ASTNode {
  public:
  ASTDeclaratorNode(TokenNode *t) : ASTNode(t, Declarator) {}
};

class ASTDeclarationNode : public ASTNode {
  public:
  vector<ASTDeclaratorNode *> declarators;
  ASTDeclarationNode(TokenNode *t) : ASTNode(t, Declaration) {}
};

class ASTFuncDeclaratorNode : public ASTNode {
  public:
  vector<ASTDeclarationNode *> args;
  ASTFuncDeclaratorNode(TokenNode *t) : ASTNode(t, FuncDeclarator) {
    args = vector<ASTDeclarationNode *>();
  }
};

class ASTFuncDeclarationNode : public ASTNode {
  public:
  ASTFuncDeclaratorNode *declarator;
  ASTFuncDeclarationNode(TokenNode *t) : ASTNode(t, FuncDeclaration) {}
};

class ASTFuncDefNode : public ASTNode {
  public:
  ASTFuncDeclarationNode *declaration;
  ASTCompoundStmtNode *body;
  ASTFuncDefNode(TokenNode *t) : ASTNode(t, FuncDef) {}
};

class ASTOtherStmtNode : public ASTNode {
  public:
  ASTOtherStmtNode(TokenNode *t, ASTType tp) : ASTNode(t, tp) {}
};

class ASTBreakStmtNode : public ASTOtherStmtNode {
  public:
  ASTBreakStmtNode(TokenNode *t) : ASTOtherStmtNode(t, BreakStmt) {}
};

class ASTContinueStmtNode : public ASTOtherStmtNode {
  public:
  ASTContinueStmtNode(TokenNode *t) : ASTOtherStmtNode(t, ContinueStmt) {}
};

class ASTExprNode : public ASTNode {
  public:
  ASTExprNode(TokenNode *t, ASTType tp) : ASTNode(t, tp) {}
};

class ASTExprStmtNode : public ASTOtherStmtNode {
  public:
  ASTExprNode *expr;
  ASTExprStmtNode(TokenNode *t) : ASTOtherStmtNode(t, ExprStmt) {}
};

class ASTReturnStmtNode : public ASTOtherStmtNode {
  public:
  ASTExprNode *expr;
  ASTReturnStmtNode(TokenNode *t) : ASTOtherStmtNode(t, ReturnStmt) {}
};

class ASTSelectionStmtNode : public ASTOtherStmtNode {
  public:
  ASTNode *cond;
  ASTCompoundStmtNode *true_stmt;
  ASTSelectionStmtNode *false_stmt;
  ASTSelectionStmtNode(TokenNode *t) : ASTOtherStmtNode(t, SelectionStmt) {}
};

class ASTLoopStmtNode : public ASTOtherStmtNode {
  public:
  ASTNode *cond;
  ASTCompoundStmtNode *true_stmt;
  ASTLoopStmtNode(TokenNode *t) : ASTOtherStmtNode(t, LoopStmt) {}
};

// parser.cpp
ASTFuncDeclaratorNode *parse_func_declarator(TokenNode **next, Error &err);
ASTFuncDeclarationNode *parse_func_declaration(TokenNode **next, Error &err);
ASTFuncDefNode *parse_func_def(TokenNode **next, Error &error);
ASTNode *parse_external_declaration(TokenNode **next, Error &err);
ASTNode *parse(TokenNode *head);