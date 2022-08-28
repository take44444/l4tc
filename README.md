# Lup Compiler
The compiler for Lup, which is the original programming language.

## Example
```
num g1, g2: 200

func add(num x, num y) -> num
  return x + y

func sub(num x, num y) -> num
  return x - y

func f(num x) -> funcp (num, num) -> num
  if x = 0
    return add
  else
    return sub

func main() -> num
  num a
  num* p: &a
  funcp (num) -> funcp (num, num) -> num fp: f
  funcp* (num) -> funcp (num, num) -> num fpp: fp

  a: 0
  g1: 100

  loop a < 2
    printf("%d\n", ((*fpp)(*p))(g1, g2))
    *p = *p + 1

  return 0
```

The output will be
```
300
-100
```

## TODO
- selection-statement
- loop-statement
- pointer

## Grammer
```
translation-unit:
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
  num pointer-list_opt
  str pointer-list_opt
  funcp pointer-list_opt (type-specifier-list) -> type-specifier

type-specifier-list:
  type-specifier
  type-specifier-list,type-specifier

pointer-list:
  *
  pointer-list *

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
  function-declaration compound-stmt

function-declaration:
  func identifier(simple-declaration-list) -> type-specifier LF

simple-declaration-list:
  simple-declaration
  simple-declaration,simple-declaration-list

simple-declaration:
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