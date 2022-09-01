# L4T Compiler
The compiler for L4T, which is the original programming language.

## Abstract
### Variable
Variable declarations can be both in local and in global. Available types are `num`, `char`, `array<>[]`, `ptr<>`, `funcptr`, `any`.

We can assign values to variables with operation "`:`".
```
num example_of_v4r_n4m3
num a, b
char c
array<num>[2] d
array<array<num>[2]>[3] e
array<char>[6] f

example_of_v4r_n4m3: 0
a: 10
b: 200
c: 'A'
d: [1, 2]
e: [[1, 2], [10, 20], [-100, -200]]
f: "Hello!"

a: d[0]
d: e[1]
b: d[0]
b: [100, -10, 20][1]
d: [[-100, -2], [0, 20000], [10, 20]][2]
c: f[3]
c: "Hi!"[1]
```

### Function
We can define functions.
```
func add(num a, num b) -> num
  return a + b
```
All types can be return type.
```
func add(num x, num y) -> num
  return x + y

func sub(num x, num y) -> num
  return x - y

func f(num x) -> funcptr (num, num) -> num
  if x = 0
    return add
  else
    return sub
```

### Pointer
Declaration and assignment.
```
num a
ptr<num> p1
ptr<ptr<num>> pp
ptr<num> p2

p1: &a
pp: &p1

a: 100
*p1: -2
**pp: 20
pp: &p2
p2: &a
a: 1000
```
All types can be pointed by pointer.
```
func add(num x, num y) -> num
  return x + y

func sub(num x, num y) -> num
  return x - y

func f(num x) -> funcptr (num, num) -> num
  if x = 0
    return add
  else
    return sub

func main() -> num
  funcptr (num) -> funcptr (num, num) -> num fp
  ptr<funcptr (num) -> funcptr (num, num) -> num> fpp

  fpp: &fp
  fp: f

  return ((*fpp)(0))(100, 200)
```

## Example
```
num g1, g2

func add(num x, num y) -> num
  return x + y

func sub(num x, num y) -> num
  return x - y

func f(num x) -> funcptr (num, num) -> num
  if x = 0
    return add
  else
    return sub

func main() -> num
  num a
  ptr<num> p
  funcptr (num) -> funcptr (num, num) -> num fp
  ptr<funcptr (num) -> funcptr (num, num) -> num> fpp

  a: 0
  p: &a
  fp: f
  fpp: &fp
  g1: 100
  g2: 200

  loop a < 2
    printf("%d\n", ((*fpp)(*p))(g1, g2))
    *p: *p + 1

  return 0
```

The output will be
```
300
-100
```

## TODO
- [x] 関数定義
- [x] 関数呼び出し
- [x] if-else文
- [ ] loop文
- [x] ローカル変数
- [x] グローバル変数
- [x] parserからのエラー出力
- [ ] generatorからのエラー出力
- [ ] 配列(文字列を含む)
- [x] 配列の値渡し
- [ ] 構造体の値渡し
- [ ] 構造体
- [x] ポインタ，関数ポインタ
- [x] 参照
- [x] 厳格な型システム．`ptr<num>`，`array<num>[2]`，`&array<num>[2]`，`&array<num>[3]`，`&array<array<num>[2]>[2]`などは全て別の型として解析され，暗黙的な変換を許さない．
- [ ] typeof
- [ ] ポインタ変数には，自身よりライフタイムが大きい変数のアドレスしか代入できないようにする．
- [ ] extern "C" func ????
- [x] libcの関数からの返り値の型はAny
- [ ] ブロックにunsafe属性を設け，型Anyはunsafeブロック内にしか存在できないようにする．
- [ ] static
- [ ] マクロ

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
  funcptr pointer-list_opt (type-specifier-list) -> type-specifier

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
  expr: expr
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