#include "bits/stdc++.h"

using namespace std;

enum ASTType {
  // expression
  expr,
  exprFuncCall,
  exprAssign,
  // statement
  exprStmt,
  selectionStmt,
  loopStmt,
  // ident
  ident,
  // declarator
  declarator,
  // func definition
  funcDefinition,
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