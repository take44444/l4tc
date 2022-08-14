#include "bits/stdc++.h"

using namespace std;

enum ASTType {
  // expression
  ASTExpr,
  ASTExprFuncCall,
  ASTEexprAssign,
  // statement
  ASTExprStmt,
  ASTSelectionStmt,
  ASTLoopStmt,
  // ident
  ASTIdent,
  // declarator
  ASTDeclarator,
  // func definition
  ASTFuncDefinition,
};

class ASTNode {
  public:
  enum ASTType type;
  ASTNode *left;
  ASTNode *right;
  ASTNode(enum ASTType tp) {
    type = tp;
  }
};