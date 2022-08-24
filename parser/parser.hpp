#ifndef PARSER_HPP
#define PARSER_HPP
#include "bits/stdc++.h"
#include "../tokenizer/tokenizer.hpp"
#include "../generator/generator.hpp"

namespace parser {
  using namespace tokenizer;
  class Error {
    public:
    Token *token;
    std::string message;
    explicit Error(std::string expected, std::string found, Token *t) : token(t) {
      message = "error: expected " + expected + ", found " + found;
    }
    std::string get_error_string() {
      std::string ret = "line:" + std::to_string(token->line) +
                        "/pos:" + std::to_string(token->pos) +
                        ": " + message + '\n';
      for (char *p = token->line_begin; *p && *p != '\n'; ret += *p++);
      ret += '\n';
      for (int i_ = 1; i_ < (int)token->pos; i_++) ret += ' ';
      ret += '^';
      for (int i_ = 1; i_ < (int)token->sv.length(); i_++) ret += '~';
      return ret;
    }
  };

  class AST {
    public:
    virtual ~AST() = default;
  };

  class ASTTypeSpec : public AST {
    public:
    Token *op;
    ASTTypeSpec(Token *t) : AST(), op(t) {}
  };

  class ASTExpr : public AST {
    public:
    std::shared_ptr<ASTExpr> left, right;
    generator::EvalType eval_type;
    ASTExpr() : AST() {}
  };

  class ASTSimpleExpr : public ASTExpr {
    public:
    Token *op;
    ASTSimpleExpr(Token *t) : ASTExpr(), op(t) {}
  };

  class ASTPrimaryExpr : public ASTExpr {
    public:
    std::shared_ptr<ASTExpr> expr;
    ASTPrimaryExpr() : ASTExpr() {}
  };

  class ASTFuncCallExpr : public ASTExpr {
    public:
    std::shared_ptr<ASTExpr> primary;
    std::vector<std::shared_ptr<ASTExpr>> args;
    ASTFuncCallExpr() : ASTExpr() {
      args = std::vector<std::shared_ptr<ASTExpr>>();
    }
  };

  class ASTMultiplicativeExpr : public ASTExpr {
    public:
    Token *op;
    ASTMultiplicativeExpr(Token *t) : ASTExpr(), op(t) {}
  };

  class ASTAdditiveExpr : public ASTExpr {
    public:
    Token *op;
    ASTAdditiveExpr(Token *t) : ASTExpr(), op(t) {}
  };

  class ASTShiftExpr : public ASTExpr {
    public:
    Token *op;
    ASTShiftExpr(Token *t) : ASTExpr(), op(t) {}
  };

  class ASTRelationalExpr : public ASTExpr {
    public:
    Token *op;
    ASTRelationalExpr(Token *t) : ASTExpr(), op(t) {}
  };

  class ASTEqualityExpr : public ASTExpr {
    public:
    Token *op;
    ASTEqualityExpr(Token *t) : ASTExpr(), op(t) {}
  };

  class ASTBitwiseAndExpr : public ASTExpr {
    public:
    ASTBitwiseAndExpr() : ASTExpr() {}
  };

  class ASTBitwiseXorExpr : public ASTExpr {
    public:
    ASTBitwiseXorExpr() : ASTExpr() {}
  };

  class ASTBitwiseOrExpr : public ASTExpr {
    public:
    ASTBitwiseOrExpr() : ASTExpr() {}
  };

  class ASTLogicalAndExpr : public ASTExpr {
    public:
    ASTLogicalAndExpr() : ASTExpr() {}
  };

  class ASTLogicalOrExpr : public ASTExpr {
    public:
    ASTLogicalOrExpr() : ASTExpr() {}
  };

  class ASTAssignExpr : public ASTExpr {
    public:
    ASTAssignExpr() : ASTExpr() {}
  };

  class ASTExprStmt : public AST {
    public:
    std::shared_ptr<AST> expr;
    ASTExprStmt() : AST() {}
  };

  class ASTBreakStmt : public AST {
    public:
    ASTBreakStmt() : AST() {}
  };

  class ASTContinueStmt : public AST {
    public:
    ASTContinueStmt() : AST() {}
  };

  class ASTReturnStmt : public AST {
    public:
    std::shared_ptr<AST> expr;
    ASTReturnStmt() : AST() {}
  };

  class ASTDeclarator : public AST {
    public:
    Token *op;
    ASTDeclarator(Token *t) : AST(), op(t) {}
  };

  class ASTDeclaration : public AST {
    public:
    std::shared_ptr<ASTTypeSpec> declaration_spec;
    std::vector<std::shared_ptr<ASTDeclarator>> declarators;
    ASTDeclaration() : AST() {
      declarators = std::vector<std::shared_ptr<ASTDeclarator>>();
    }
  };

  class ASTSimpleDeclaration : public AST {
    public:
    std::shared_ptr<ASTTypeSpec> type_spec;
    std::shared_ptr<ASTDeclarator> declarator;
    ASTSimpleDeclaration() : AST() {}
  };

  class ASTCompoundStmt : public AST {
    public:
    std::vector<std::shared_ptr<AST>> items;
    ASTCompoundStmt() : AST() {
      items = std::vector<std::shared_ptr<AST>>();
    }
  };

  class ASTFuncDeclarator : public AST {
    public:
    std::shared_ptr<ASTDeclarator> declarator;
    std::vector<std::shared_ptr<ASTSimpleDeclaration>> args;
    ASTFuncDeclarator() : AST() {
      args = std::vector<std::shared_ptr<ASTSimpleDeclaration>>();
    }
  };

  class ASTFuncDeclaration : public AST {
    public:
    std::shared_ptr<ASTTypeSpec> type_spec;
    std::shared_ptr<ASTFuncDeclarator> declarator;
    ASTFuncDeclaration() : AST() {}
  };

  class ASTFuncDef : public AST {
    public:
    std::shared_ptr<ASTFuncDeclaration> declaration;
    std::shared_ptr<ASTCompoundStmt> body;
    ASTFuncDef() : AST() {}
  };

  class ASTExternalDeclaration : public AST {
    public:
    std::shared_ptr<ASTTypeSpec> declaration_spec;
    std::vector<std::shared_ptr<ASTDeclarator>> declarators;
    ASTExternalDeclaration() : AST() {
      declarators = std::vector<std::shared_ptr<ASTDeclarator>>();
    }
  };

  class ASTTranslationUnit : public AST {
    public:
    std::vector<std::shared_ptr<AST>> external_declarations;
    ASTTranslationUnit() : AST() {
      external_declarations = std::vector<std::shared_ptr<AST>>();
    }
  };

  bool is_unary_expr(std::shared_ptr<AST> node);

  // parser.cpp
  std::shared_ptr<ASTTranslationUnit> parse(Token **head, Error &err);
  void print_ast(std::shared_ptr<AST> n);

  // utils.cpp
  Token *expect_token_with_str(Token **next, Error &err, std::string str);
  Token *consume_token_with_str(Token **next, std::string str);
  Token *expect_token_with_type(Token **next, Error &err, TokenType type);
  Token *consume_token_with_type(Token **next, TokenType type);
  Token *consume_token_with_indents(Token **next, int indents);
  Token *expect_token_with_indents(Token **next, Error &err, int indents);
}
#endif