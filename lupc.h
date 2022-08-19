#include "bits/stdc++.h"

using namespace std;

class Error {
  public:
  string err_str;
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

class Token {
  public:
  enum TokenType type;
  Token *next;
  char *begin;
  int length;
  int line;
  Token(int l, char *beg, int len, enum TokenType tp) {
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
Token *expect_token_with_str(Token **next, Error &err, string str);
Token *consume_token_with_str(Token **next, string str);
Token *expect_token_with_type(Token **next, Error &err, TokenType type);
Token *consume_token_with_type(Token **next, TokenType type);
Token *consume_token_with_indents(Token **next, int indents);
Token *expect_token_with_indents(Token **next, Error &err, int indents);

// tokenizer.cpp
Token *create_next_token(char *p, int &line);
void print_tokens(Token *head);
Token *tokenize(string &source);

enum ASTType {
  TypeSpec,
  SimpleExpr,
  PrimaryExpr,
  FuncCallExpr,
  MultiplicativeExpr,
  AdditiveExpr,
  ShiftExpr,
  RelationalExpr,
  EqualityExpr,
  BitwiseAndExpr,
  BitwiseXorExpr,
  BitwiseOrExpr,
  LogicalAndExpr,
  LogicalOrExpr,
  AssignExpr,
  ExprStmt,
  BreakStmt,
  ContinueStmt,
  ReturnStmt,
  Declarator,
  Declaration,
  CompoundStmt,
  FuncDeclarator,
  FuncDeclaration,
  FuncDef,
  ExternalDeclaration,
  TranslationUnit,

  // SelectionStmt,
  // LoopStmt,
};

class AST {
  public:
  ASTType type;
  AST(ASTType tp) : type(tp) {}
  bool is_unary_expr() {
    return (
      type == SimpleExpr ||
      type == PrimaryExpr ||
      type == FuncCallExpr
    );
  }
};

class ASTTypeSpec : public AST {
  public:
  Token *op;
  ASTTypeSpec(Token *t) : AST(TypeSpec), op(t) {}
};

class ASTExpr : public AST {
  public:
  shared_ptr<ASTExpr> left, right;
  ASTExpr(ASTType tp) : AST(tp) {}
};

class ASTSimpleExpr : public ASTExpr {
  public:
  Token *op;
  ASTSimpleExpr(Token *t) : ASTExpr(SimpleExpr), op(t) {}
};

class ASTPrimaryExpr : public ASTExpr {
  public:
  shared_ptr<ASTExpr> expr;
  ASTPrimaryExpr() : ASTExpr(PrimaryExpr) {}
};

class ASTFuncCallExpr : public ASTExpr {
  public:
  shared_ptr<ASTExpr> primary;
  vector<shared_ptr<ASTExpr>> args;
  ASTFuncCallExpr() : ASTExpr(FuncCallExpr) {
    args = vector<shared_ptr<ASTExpr>>();
  }
};

class ASTMultiplicativeExpr : public ASTExpr {
  public:
  ASTMultiplicativeExpr() : ASTExpr(MultiplicativeExpr) {}
};

class ASTAdditiveExpr : public ASTExpr {
  public:
  ASTAdditiveExpr() : ASTExpr(AdditiveExpr) {}
};

class ASTShiftExpr : public ASTExpr {
  public:
  ASTShiftExpr() : ASTExpr(ShiftExpr) {}
};

class ASTRelationalExpr : public ASTExpr {
  public:
  ASTRelationalExpr() : ASTExpr(RelationalExpr) {}
};

class ASTEqualityExpr : public ASTExpr {
  public:
  ASTEqualityExpr() : ASTExpr(EqualityExpr) {}
};

class ASTBitwiseAndExpr : public ASTExpr {
  public:
  ASTBitwiseAndExpr() : ASTExpr(BitwiseAndExpr) {}
};

class ASTBitwiseXorExpr : public ASTExpr {
  public:
  ASTBitwiseXorExpr() : ASTExpr(BitwiseXorExpr) {}
};

class ASTBitwiseOrExpr : public ASTExpr {
  public:
  ASTBitwiseOrExpr() : ASTExpr(BitwiseOrExpr) {}
};

class ASTLogicalAndExpr : public ASTExpr {
  public:
  ASTLogicalAndExpr() : ASTExpr(LogicalAndExpr) {}
};

class ASTLogicalOrExpr : public ASTExpr {
  public:
  ASTLogicalOrExpr() : ASTExpr(LogicalOrExpr) {}
};

class ASTAssignExpr : public ASTExpr {
  public:
  ASTAssignExpr() : ASTExpr(AssignExpr) {}
};

class ASTExprStmt : public AST {
  public:
  shared_ptr<AST> expr;
  ASTExprStmt() : AST(ExprStmt) {}
};

class ASTBreakStmt : public AST {
  public:
  ASTBreakStmt() : AST(BreakStmt) {}
};

class ASTContinueStmt : public AST {
  public:
  ASTContinueStmt() : AST(ContinueStmt) {}
};

class ASTReturnStmt : public AST {
  public:
  shared_ptr<AST> expr;
  ASTReturnStmt() : AST(ReturnStmt) {}
};

class ASTDeclarator : public AST {
  public:
  Token *op;
  ASTDeclarator(Token *t) : AST(Declarator), op(t) {}
};

class ASTDeclaration : public AST {
  public:
  shared_ptr<ASTTypeSpec> declaration_spec;
  vector<shared_ptr<ASTDeclarator>> declarators;
  ASTDeclaration() : AST(Declaration) {
    declarators = vector<shared_ptr<ASTDeclarator>>();
  }
};

class ASTSimpleDeclaration : public AST {
  public:
  shared_ptr<ASTTypeSpec> type_spec;
  shared_ptr<ASTDeclarator> declarator;
  ASTSimpleDeclaration() : AST(Declaration) {}
};

class ASTCompoundStmt : public AST {
  public:
  vector<shared_ptr<AST>> items;
  ASTCompoundStmt() : AST(CompoundStmt) {
    items = vector<shared_ptr<AST>>();
  }
};

class ASTFuncDeclarator : public AST {
  public:
  shared_ptr<ASTDeclarator> declarator;
  vector<shared_ptr<ASTSimpleDeclaration>> args;
  ASTFuncDeclarator() : AST(FuncDeclarator) {
    args = vector<shared_ptr<ASTSimpleDeclaration>>();
  }
};

class ASTFuncDeclaration : public AST {
  public:
  shared_ptr<ASTTypeSpec> type_spec;
  shared_ptr<ASTFuncDeclarator> declarator;
  ASTFuncDeclaration() : AST(FuncDeclaration) {}
};

class ASTFuncDef : public AST {
  public:
  shared_ptr<ASTFuncDeclaration> declaration;
  shared_ptr<ASTCompoundStmt> body;
  ASTFuncDef() : AST(FuncDef) {}
};

class ASTExternalDeclaration : public AST {
  public:
  shared_ptr<ASTTypeSpec> declaration_spec;
  vector<shared_ptr<ASTDeclarator>> declarators;
  ASTExternalDeclaration() : AST(ExternalDeclaration) {
    declarators = vector<shared_ptr<ASTDeclarator>>();
  }
};

class ASTTranslationUnit : public AST {
  public:
  vector<shared_ptr<AST>> external_declarations;
  ASTTranslationUnit() : AST(TranslationUnit) {
    external_declarations = vector<shared_ptr<AST>>();
  }
};

// parser.cpp
shared_ptr<ASTTranslationUnit> parse(Token **head, Error &err);