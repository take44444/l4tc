#include "bits/stdc++.h"
#include "token.cpp"

using namespace std;

TokenNode *create_next_token(const char *p, int &line) {
  assert(line);
  if (!*p) return NULL;
  if (*p == '\n') {
    line++;
    return new TokenNode(line, p, 1, delimiter);
  }
  if ('1' <= *p && *p <= '9') {
    int length = 0;
    while ('0' <= p[length] && p[length] <= '9') length++;
    return new TokenNode(line, p, length, numberConstant);
  }
  if (('A' <= *p && *p <= 'Z') || ('a' <= *p && *p <= 'z') || *p == '_') {
    int length = 0;
    while (('A' <= p[length] && p[length] <= 'Z') ||
           ('a' <= p[length] && p[length] <= 'z') || p[length] == '_' ||
           ('0' <= p[length] && p[length] <= '9')) length++;
    TokenNode *ret = new TokenNode(line, p, length, ident);
    if (ret->is_equal_with_cstr("break")) ret->type = kwBreak;
    else if (ret->is_equal_with_cstr("continue")) ret->type = kwContinue;
    else if (ret->is_equal_with_cstr("elif")) ret->type = kwElif;
    else if (ret->is_equal_with_cstr("else")) ret->type = kwElse;
    else if (ret->is_equal_with_cstr("for")) ret->type = kwFor;
    else if (ret->is_equal_with_cstr("func")) ret->type = kwFunc;
    else if (ret->is_equal_with_cstr("if")) ret->type = kwIf;
    else if (ret->is_equal_with_cstr("loop")) ret->type = kwLoop;
    else if (ret->is_equal_with_cstr("num")) ret->type = kwNum;
    else if (ret->is_equal_with_cstr("return")) ret->type = kwReturn;
    else if (ret->is_equal_with_cstr("str")) ret->type = kwStr;
    else if (ret->is_equal_with_cstr("void")) ret->type = kwVoid;
    return ret;
  }
  if ('&' == *p) {
    if (p[1] == '&') return new TokenNode(line, p, 2, punctuator);
    return new TokenNode(line, p, 1, punctuator);
  }
  if ('|' == *p) {
    if (p[1] == '|') return new TokenNode(line, p, 2, punctuator);
    return new TokenNode(line, p, 1, punctuator);
  }
  if ('<' == *p) {
    if (p[1] == '<') return new TokenNode(line, p, 2, punctuator);
    if (p[1] == '=') return new TokenNode(line, p, 2, punctuator);
    if (p[1] == '-') return new TokenNode(line, p, 2, punctuator);
    return new TokenNode(line, p, 1, punctuator);
  }
  if ('>' == *p) {
    if (p[1] == '>') return new TokenNode(line, p, 2, punctuator);
    if (p[1] == '=') return new TokenNode(line, p, 2, punctuator);
    return new TokenNode(line, p, 1, punctuator);
  }
  if ('!' == *p) {
    if (p[1] == '=') return new TokenNode(line, p, 2, punctuator);
  }
  if ('=' == *p ||
      '^' == *p || '~' == *p ||
      '+' == *p ||
      '/' == *p || '*' == *p || '%' == *p ||
      '(' == *p || ')' == *p ||
      '[' == *p || ']' == *p ||
      ',' == *p) {
    return new TokenNode(line, p, 1, punctuator);
  }
  if ('-' == *p) {
    if (p[1] == '>') return new TokenNode(line, p, 2, punctuator);
    return new TokenNode(line, p, 1, punctuator);
  }
  return new TokenNode(line, p, 1, unknown);
}

void print_token(TokenNode *head) {
  cout << "Not Implemented" << endl;
}

TokenNode *tokenize(const char *input) {
  TokenNode *head = NULL;
  TokenNode **next = &head;
  TokenNode *t;
  int l = 1;
  const char *p = input;
  while (t = create_next_token(p, l)) {
    next = &t->next;
    p = t->begin + t->length;
  }
  return head;
}