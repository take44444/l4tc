#include "bits/stdc++.h"

using namespace std;

enum TokenType {
  delimiter,      //  
  punctuator,     // , ( ) \n 
  numberConstant, // 0 03 0x07 0b10
  stringLiteral,  // "hello world"
  ident,          // vAr_n4m3
  kwBreak,        // break
  kwContinue,     // continue
  kwElif,         // elif
  kwElse,         // else
  kwFor,          // for
  kwFunc,         // func
  kwIf,           // if
  kwLoop,         // loop
  kwNum,          // num
  kwReturn,       // return
  kwStr,          // str
  kwVoid,         // void
  unknown,        // unknown
};

class TokenNode {
  public:
  enum TokenType type;
  TokenNode *next;
  const char *begin;
  int length;
  int line;
  TokenNode(int l, const char *beg, int len, enum TokenType tp) {
    line = l;
    begin = beg;
    length = len;
    type = tp;
    next = NULL;
  }

  bool is_equal_with_cstr(const char *s) {
    return strlen(s) == length && strncmp(begin, s, length) == 0;
  }

  void print() {
    fprintf(stdout, "%.*s", length, begin);
  }
};