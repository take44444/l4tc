# Lup Compiler
The compiler for Lup which is the original programming language.

## Grammer

```
external-declaration:
  declaration
  function-defintition

declaration:
  type-specifier declarator-list

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

declarator:
  identifier

function-definition:
  function-declaration compound-stmt

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
```