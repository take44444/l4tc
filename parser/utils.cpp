#include "./parser.hpp"
namespace parser {
  tokenizer::Token *expect_token_with_str(tokenizer::Token **next, Error &err, std::string str) {
    // NULL check
    if (!*next) {
      err = Error("expected ????, found EOF");
      return NULL;
    }
    if ((*next)->sv != str) {
      err = Error("expected ????, found ????");
      return NULL;
    }
    tokenizer::Token *ret = *next;
    *next = (*next)->next;
    return ret;
  }

  tokenizer::Token *consume_token_with_str(tokenizer::Token **next, std::string str) {
    if (!*next) return NULL;
    if ((*next)->sv != str) return NULL;
    tokenizer::Token *ret = *next;
    *next = (*next)->next;
    return ret;
  }

  tokenizer::Token *expect_token_with_type(tokenizer::Token **next, Error &err, tokenizer::TokenType type) {
    if (!*next) {
      err = Error("expected ????, found EOF");
      return NULL;
    }
    if ((*next)->type != type) {
      err = Error("expected ????, found ????");
      return NULL;
    }
    tokenizer::Token *ret = *next;
    *next = (*next)->next;
    return ret;
  }

  tokenizer::Token *consume_token_with_type(tokenizer::Token **next, tokenizer::TokenType type) {
    if (!*next) return NULL;
    if ((*next)->type != type) return NULL;
    tokenizer::Token *ret = *next;
    *next = (*next)->next;
    return ret;
  }

  tokenizer::Token *consume_token_with_indents(tokenizer::Token **next, int indents) {
    if (!*next) return NULL;
    if ((*next)->sv.front() != ' ' || (int)(*next)->sv.length() != indents) return NULL;
    tokenizer::Token *ret = *next;
    *next = (*next)->next;
    return ret;
  }

  template <class T> void print_ast_vec(std::vector<std::shared_ptr<T>> &v, int depth) {
    std::cerr << '[';
    if (v.size() == 0) {
      std::cerr << ']';
      return;
    }
    for (int i=0; i < (int)v.size(); i++) {
      std::cerr << (i ? ",\n" : "\n");
      for (int j_ = 0; j_ < depth + 1; j_++) std::cerr << ' ';
      print_ast_sub(v.at(i), depth + 1);
    }
    std::cerr << std::endl;
    for (int i_ = 0; i_ < depth; i_++) std::cerr << ' ';
    std::cerr << ']';
  }

  void print_ast_sub(std::shared_ptr<AST> n, int depth) {
    if (n->type == TypeSpec) {
      std::shared_ptr<ASTTypeSpec> nn = std::dynamic_pointer_cast<ASTTypeSpec>(n);
      std::cerr << "TypeSpec<" << nn->op->sv << '>';
      return;
    }
    if (n->type == SimpleExpr) {
      std::shared_ptr<ASTSimpleExpr> nn = std::dynamic_pointer_cast<ASTSimpleExpr>(n);
      std::cerr << "SimpleExpr<" << nn->op->sv << '>';
      return;
    }
    if (n->type == PrimaryExpr) {
      std::shared_ptr<ASTPrimaryExpr> nn = std::dynamic_pointer_cast<ASTPrimaryExpr>(n);
      std::cerr << "PrimaryExpr(e=";
      print_ast_sub(nn->expr, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == FuncCallExpr) {
      std::shared_ptr<ASTFuncCallExpr> nn = std::dynamic_pointer_cast<ASTFuncCallExpr>(n);
      std::cerr << "FuncCallExpr(p=";
      print_ast_sub(nn->primary, depth);
      std::cerr << ", args=";
      print_ast_vec(nn->args, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == MultiplicativeExpr) {
      std::shared_ptr<ASTMultiplicativeExpr> nn = std::dynamic_pointer_cast<ASTMultiplicativeExpr>(n);
      std::cerr << "MultiplicativeExpr(l=";
      print_ast_sub(nn->left, depth);
      std::cerr << ", r=";
      print_ast_sub(nn->right, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == AdditiveExpr) {
      std::shared_ptr<ASTAdditiveExpr> nn = std::dynamic_pointer_cast<ASTAdditiveExpr>(n);
      std::cerr << "AdditiveExpr(l=";
      print_ast_sub(nn->left, depth);
      std::cerr << ", r=";
      print_ast_sub(nn->right, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == ShiftExpr) {
      std::shared_ptr<ASTShiftExpr> nn = std::dynamic_pointer_cast<ASTShiftExpr>(n);
      std::cerr << "ShiftExpr(l=";
      print_ast_sub(nn->left, depth);
      std::cerr << ", r=";
      print_ast_sub(nn->right, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == RelationalExpr) {
      std::shared_ptr<ASTRelationalExpr> nn = std::dynamic_pointer_cast<ASTRelationalExpr>(n);
      std::cerr << "RelationalExpr(l=";
      print_ast_sub(nn->left, depth);
      std::cerr << ", r=";
      print_ast_sub(nn->right, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == EqualityExpr) {
      std::shared_ptr<ASTEqualityExpr> nn = std::dynamic_pointer_cast<ASTEqualityExpr>(n);
      std::cerr << "EqualityExpr(l=";
      print_ast_sub(nn->left, depth);
      std::cerr << ", r=";
      print_ast_sub(nn->right, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == BitwiseAndExpr) {
      std::shared_ptr<ASTBitwiseAndExpr> nn = std::dynamic_pointer_cast<ASTBitwiseAndExpr>(n);
      std::cerr << "BitwiseAndExpr(l=";
      print_ast_sub(nn->left, depth);
      std::cerr << ", r=";
      print_ast_sub(nn->right, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == BitwiseXorExpr) {
      std::shared_ptr<ASTBitwiseXorExpr> nn = std::dynamic_pointer_cast<ASTBitwiseXorExpr>(n);
      std::cerr << "BitwiseXorExpr(l=";
      print_ast_sub(nn->left, depth);
      std::cerr << ", r=";
      print_ast_sub(nn->right, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == BitwiseOrExpr) {
      std::shared_ptr<ASTBitwiseOrExpr> nn = std::dynamic_pointer_cast<ASTBitwiseOrExpr>(n);
      std::cerr << "BitwiseOrExpr(l=";
      print_ast_sub(nn->left, depth);
      std::cerr << ", r=";
      print_ast_sub(nn->right, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == LogicalAndExpr) {
      std::shared_ptr<ASTLogicalAndExpr> nn = std::dynamic_pointer_cast<ASTLogicalAndExpr>(n);
      std::cerr << "LogicalAndExpr(l=";
      print_ast_sub(nn->left, depth);
      std::cerr << ", r=";
      print_ast_sub(nn->right, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == LogicalOrExpr) {
      std::shared_ptr<ASTLogicalOrExpr> nn = std::dynamic_pointer_cast<ASTLogicalOrExpr>(n);
      std::cerr << "LogicalOrExpr(l=";
      print_ast_sub(nn->left, depth);
      std::cerr << ", r=";
      print_ast_sub(nn->right, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == AssignExpr) {
      std::shared_ptr<ASTAssignExpr> nn = std::dynamic_pointer_cast<ASTAssignExpr>(n);
      std::cerr << "AssignExpr(l=";
      print_ast_sub(nn->left, depth);
      std::cerr << ", r=";
      print_ast_sub(nn->right, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == ExprStmt) {
      std::shared_ptr<ASTExprStmt> nn = std::dynamic_pointer_cast<ASTExprStmt>(n);
      std::cerr << "ExprStmt(expr=";
      print_ast_sub(nn->expr, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == BreakStmt) {
      std::shared_ptr<ASTBreakStmt> nn = std::dynamic_pointer_cast<ASTBreakStmt>(n);
      std::cerr << "BreakStmt";
      return;
    }
    if (n->type == ContinueStmt) {
      std::shared_ptr<ASTContinueStmt> nn = std::dynamic_pointer_cast<ASTContinueStmt>(n);
      std::cerr << "ContinueStmt";
      return;
    }
    if (n->type == ReturnStmt) {
      std::shared_ptr<ASTReturnStmt> nn = std::dynamic_pointer_cast<ASTReturnStmt>(n);
      std::cerr << "ReturnStmt(expr=";
      print_ast_sub(nn->expr, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == Declarator) {
      std::shared_ptr<ASTDeclarator> nn = std::dynamic_pointer_cast<ASTDeclarator>(n);
      std::cerr << "Declarator<" << nn->op->sv << '>';
      return;
    }
    if (n->type == Declaration) {
      std::shared_ptr<ASTDeclaration> nn = std::dynamic_pointer_cast<ASTDeclaration>(n);
      std::cerr << "Declaration(ds=";
      print_ast_sub(nn->declaration_spec, depth);
      std::cerr << ", d-list=";
      print_ast_vec(nn->declarators, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == SimpleDeclaration) {
      std::shared_ptr<ASTSimpleDeclaration> nn = std::dynamic_pointer_cast<ASTSimpleDeclaration>(n);
      std::cerr << "SimpleDeclaration(ts=";
      print_ast_sub(nn->type_spec, depth);
      std::cerr << ", d=";
      print_ast_sub(nn->declarator, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == CompoundStmt) {
      std::shared_ptr<ASTCompoundStmt> nn = std::dynamic_pointer_cast<ASTCompoundStmt>(n);
      std::cerr << "CompoundStmt(item-list=";
      print_ast_vec(nn->items, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == FuncDeclarator) {
      std::shared_ptr<ASTFuncDeclarator> nn = std::dynamic_pointer_cast<ASTFuncDeclarator>(n);
      std::cerr << "FuncDeclarator(d=";
      print_ast_sub(nn->declarator, depth);
      std::cerr << ", args=";
      print_ast_vec(nn->args, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == FuncDeclaration) {
      std::shared_ptr<ASTFuncDeclaration> nn = std::dynamic_pointer_cast<ASTFuncDeclaration>(n);
      std::cerr << "FuncDeclaration(ts=";
      print_ast_sub(nn->type_spec, depth);
      std::cerr << ", d=";
      print_ast_sub(nn->declarator, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == FuncDef) {
      std::shared_ptr<ASTFuncDef> nn = std::dynamic_pointer_cast<ASTFuncDef>(n);
      std::cerr << "FuncDef(d=";
      print_ast_sub(nn->declaration, depth);
      std::cerr << ", body=";
      print_ast_sub(nn->body, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == ExternalDeclaration) {
      std::shared_ptr<ASTExternalDeclaration> nn = std::dynamic_pointer_cast<ASTExternalDeclaration>(n);
      std::cerr << "ExternalDeclaration(ds=";
      print_ast_sub(nn->declaration_spec, depth);
      std::cerr << ", d-list=";
      print_ast_vec(nn->declarators, depth);
      std::cerr << ')';
      return;
    }
    if (n->type == TranslationUnit) {
      std::shared_ptr<ASTTranslationUnit> nn = std::dynamic_pointer_cast<ASTTranslationUnit>(n);
      std::cerr << "TranslationUnit(ed-list=";
      print_ast_vec(nn->external_declarations, depth);
      std::cerr << ')';
      return;
    }
  }

  void print_ast(std::shared_ptr<AST> n) {
    print_ast_sub(n, 0);
    std::cerr << std::endl;
  }
}