#include "bits/stdc++.h"

using namespace std;

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
  KwFor,          // for
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

class TokenNode {
  public:
  enum TokenType type;
  TokenNode *next;
  char *begin;
  int length;
  int line;
  TokenNode(int l, char *beg, int len, enum TokenType tp) {
    line = l;
    begin = beg;
    length = len;
    type = tp;
    next = NULL;
  }

  bool is_equal_with_cstr(const char *s) {
    return strlen(s) == (size_t)length && strncmp(begin, s, length) == 0;
  }

  void print() {
    fprintf(stdout, "%.*s", length, begin);
  }
};

// tokenizer.cpp
TokenNode *create_next_token(char *p, int &line);
void print_tokens(TokenNode *head);
TokenNode *tokenize(string &source);