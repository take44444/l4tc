#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP
#include "bits/stdc++.h"

namespace tokenizer {
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

  class Token {
    public:
    int line;
    std::string_view sv;
    enum TokenType type;
    Token *next;
    Token(int l, std::string_view s, enum TokenType tp)
    : line(l), sv(s), type(tp), next(NULL) {}
    std::string to_string() {
      switch (next->type)
      {
        case Delimiter:
          return "Delimiter";
        case Punctuator:
          return "Punctuator";
        case NumberConstant:
          return "NumberConstant";
        case StringLiteral:
          return "StringLiteral";
        case Ident:
          return "Ident";
        case KwBreak:
          return "KwBreak";
        case KwContinue:
          return "KwContinue";
        case KwElif:
          return "KwElif";
        case KwElse:
          return "KwElse";
        case KwFunc:
          return "KwFunc";
        case KwIf:
          return "KwIf";
        case KwLoop:
          return "KwLoop";
        case KwNum:
          return "KwNum";
        case KwReturn:
          return "KwReturn";
        case KwStr:
          return "KwStr";
        case KwVoid:
          return "KwVoid";
        case Unknown:
          return "Unknown";
        default:
          return "????";
      }
    }
  };

  // tokenizer.cpp
  Token *create_next_token(char *p, int &line);
  void print_tokens(Token *head);
  Token *tokenize(std::string &source);
}
#endif