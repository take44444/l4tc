#ifndef PARSER_HPP
#define PARSER_HPP
#include "bits/stdc++.h"
#include "../tokenizer/tokenizer.hpp"

namespace parser {
  class Error {
    public:
    std::string str;
    Error(std::string err) {
      str = err;
    }
  };

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
    SimpleDeclaration,
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
    virtual ~AST() = default;
  };

  class ASTTypeSpec : public AST {
    public:
    tokenizer::Token *op;
    ASTTypeSpec(tokenizer::Token *t) : AST(TypeSpec), op(t) {}
  };

  class ASTExpr : public AST {
    public:
    std::shared_ptr<ASTExpr> left, right;
    ASTExpr(ASTType tp) : AST(tp) {}
  };

  class ASTSimpleExpr : public ASTExpr {
    public:
    tokenizer::Token *op;
    ASTSimpleExpr(tokenizer::Token *t) : ASTExpr(SimpleExpr), op(t) {}
  };

  class ASTPrimaryExpr : public ASTExpr {
    public:
    std::shared_ptr<ASTExpr> expr;
    ASTPrimaryExpr() : ASTExpr(PrimaryExpr) {}
  };

  class ASTFuncCallExpr : public ASTExpr {
    public:
    std::shared_ptr<ASTExpr> primary;
    std::vector<std::shared_ptr<ASTExpr>> args;
    ASTFuncCallExpr() : ASTExpr(FuncCallExpr) {
      args = std::vector<std::shared_ptr<ASTExpr>>();
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
    std::shared_ptr<AST> expr;
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
    std::shared_ptr<AST> expr;
    ASTReturnStmt() : AST(ReturnStmt) {}
  };

  class ASTDeclarator : public AST {
    public:
    tokenizer::Token *op;
    ASTDeclarator(tokenizer::Token *t) : AST(Declarator), op(t) {}
  };

  class ASTDeclaration : public AST {
    public:
    std::shared_ptr<ASTTypeSpec> declaration_spec;
    std::vector<std::shared_ptr<ASTDeclarator>> declarators;
    ASTDeclaration() : AST(Declaration) {
      declarators = std::vector<std::shared_ptr<ASTDeclarator>>();
    }
  };

  class ASTSimpleDeclaration : public AST {
    public:
    std::shared_ptr<ASTTypeSpec> type_spec;
    std::shared_ptr<ASTDeclarator> declarator;
    ASTSimpleDeclaration() : AST(SimpleDeclaration) {}
  };

  class ASTCompoundStmt : public AST {
    public:
    std::vector<std::shared_ptr<AST>> items;
    ASTCompoundStmt() : AST(CompoundStmt) {
      items = std::vector<std::shared_ptr<AST>>();
    }
  };

  class ASTFuncDeclarator : public AST {
    public:
    std::shared_ptr<ASTDeclarator> declarator;
    std::vector<std::shared_ptr<ASTSimpleDeclaration>> args;
    ASTFuncDeclarator() : AST(FuncDeclarator) {
      args = std::vector<std::shared_ptr<ASTSimpleDeclaration>>();
    }
  };

  class ASTFuncDeclaration : public AST {
    public:
    std::shared_ptr<ASTTypeSpec> type_spec;
    std::shared_ptr<ASTFuncDeclarator> declarator;
    ASTFuncDeclaration() : AST(FuncDeclaration) {}
  };

  class ASTFuncDef : public AST {
    public:
    std::shared_ptr<ASTFuncDeclaration> declaration;
    std::shared_ptr<ASTCompoundStmt> body;
    ASTFuncDef() : AST(FuncDef) {}
  };

  class ASTExternalDeclaration : public AST {
    public:
    std::shared_ptr<ASTTypeSpec> declaration_spec;
    std::vector<std::shared_ptr<ASTDeclarator>> declarators;
    ASTExternalDeclaration() : AST(ExternalDeclaration) {
      declarators = std::vector<std::shared_ptr<ASTDeclarator>>();
    }
  };

  class ASTTranslationUnit : public AST {
    public:
    std::vector<std::shared_ptr<AST>> external_declarations;
    ASTTranslationUnit() : AST(TranslationUnit) {
      external_declarations = std::vector<std::shared_ptr<AST>>();
    }
  };

  // parser.cpp
  std::shared_ptr<ASTTranslationUnit> parse(tokenizer::Token **head, Error &err);
  void print_ast(std::shared_ptr<AST> n);

  // utils.cpp
  tokenizer::Token *expect_token_with_str(tokenizer::Token **next, Error &err, std::string str);
  tokenizer::Token *consume_token_with_str(tokenizer::Token **next, std::string str);
  tokenizer::Token *expect_token_with_type(tokenizer::Token **next, Error &err, tokenizer::TokenType type);
  tokenizer::Token *consume_token_with_type(tokenizer::Token **next, tokenizer::TokenType type);
  tokenizer::Token *consume_token_with_indents(tokenizer::Token **next, int indents);
  tokenizer::Token *expect_token_with_indents(tokenizer::Token **next, Error &err, int indents);
}
#endif