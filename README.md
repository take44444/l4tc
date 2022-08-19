# Lup Compiler
The compiler for Lup, which is the original programming language.

## TODO
- make analyzer
- make generator

## Grammer
```
code:
  external-declaration
  code external-declaration

external-declaration:
  declaration
  function-definition

declaration:
  declaration-specifier declarator-list LF

declaration-specifier:
  type-specifier
  type-qualifier type-specifier
  storage-class-specifier type-specifier

type-specifier:
  num
  str
  void

type-qualifier:
  const

storage-class-specifier:
  static

declarator-list:
  init-declarator
  declarator-list,init-declarator

init-declarator:
  declarator: expr
  declarator

declarator:
  identifier // 変数

expr:
  identifier // 変数
  number-constant // 数値
  expr + expr
  expr - expr
  expr / expr
  expr * expr
  expr & expr
  expr | expr
  expr < expr
  expr > expr
  expr <= expr
  expr >= expr
  expr != expr
  expr <- expr
  expr -> expr
  (expr)
  identifier()
  identifier(expr-list)

expr-list:
  expr
  expr-list,expr

function-definition:
  function-declaration LF compound-stmt

function-declaration:
  func identifier(single-declaration-list) -> type-specifier

single-declaration-list:
  single-declaration
  single-declaration,single-declaration-list

single-declaration:
  type-specifier declarator

compound-stmt:
  indents item
  compound-stmt indents item

indents:
  2spaces

item:
  declaration
  statement

statement:
  expr-stmt
  compound-stmt
  return-stmt
  break-stmt
  continue-stmt
  selection-stmt
  loop-stmt

expr-stmt:
  expr LF

return-stmt:
  return expr LF

break-stmt:
  break LF

continue-stmt:
  continue LF

selection-stmt:
  if expr LF compound-stmt
  if expr LF compound-stmt selection-elif-stmt
  if expr LF compound-stmt selection-elif-stmt selection-else-stmt
  if expr LF compound-stmt selection-else-stmt

selection-elif-stmt:
  elif expr LF compound-stmt
  selection-else-stmt elif expr LF compound-stmt

selection-else-stmt:
  else LF compound-stmt

loop-stmt:
  loop expr LF compound-stmt
```