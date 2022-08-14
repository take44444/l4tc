#include "lupc.h"

TokenNode *create_next_token(char *p, int &line) {
  assert(line);
  if (!*p) return NULL;
  if (*p == ' ') {
    return new TokenNode(line, p, 1, Delimiter);
  }
  if ('1' <= *p && *p <= '9') {
    int length = 0;
    while ('0' <= p[length] && p[length] <= '9') length++;
    return new TokenNode(line, p, length, NumberConstant);
  }
  if (('A' <= *p && *p <= 'Z') || ('a' <= *p && *p <= 'z') || *p == '_') {
    int length = 0;
    while (('A' <= p[length] && p[length] <= 'Z') ||
           ('a' <= p[length] && p[length] <= 'z') || p[length] == '_' ||
           ('0' <= p[length] && p[length] <= '9')) length++;
    TokenNode *ret = new TokenNode(line, p, length, Ident);
    if (ret->is_equal_with_cstr("break")) ret->type = KwBreak;            // break
    else if (ret->is_equal_with_cstr("continue")) ret->type = KwContinue; // continue
    else if (ret->is_equal_with_cstr("elif")) ret->type = KwElif;         // elif
    else if (ret->is_equal_with_cstr("else")) ret->type = KwElse;         // else
    else if (ret->is_equal_with_cstr("for")) ret->type = KwFor;           // for
    else if (ret->is_equal_with_cstr("func")) ret->type = KwFunc;         // func
    else if (ret->is_equal_with_cstr("if")) ret->type = KwIf;             // if
    else if (ret->is_equal_with_cstr("loop")) ret->type = KwLoop;         // loop
    else if (ret->is_equal_with_cstr("num")) ret->type = KwNum;           // num
    else if (ret->is_equal_with_cstr("return")) ret->type = KwReturn;     // return
    else if (ret->is_equal_with_cstr("str")) ret->type = KwStr;           // str
    else if (ret->is_equal_with_cstr("void")) ret->type = KwVoid;         // void
    return ret;
  }
  if ('!' == *p && p[1] == '=') {
    return new TokenNode(line, p, 2, Punctuator);                         // !=
  }
  if ('&' == *p) {
    if (p[1] == '&') return new TokenNode(line, p, 2, Punctuator);        // &&
    return new TokenNode(line, p, 1, Punctuator);                         // &
  }
  if ('|' == *p) {
    if (p[1] == '|') return new TokenNode(line, p, 2, Punctuator);        // ||
    return new TokenNode(line, p, 1, Punctuator);                         // |
  }
  if ('<' == *p) {
    if (p[1] == '<') return new TokenNode(line, p, 2, Punctuator);        // <<
    if (p[1] == '=') return new TokenNode(line, p, 2, Punctuator);        // <=
    if (p[1] == '-') return new TokenNode(line, p, 2, Punctuator);        // <-
    return new TokenNode(line, p, 1, Punctuator);
  }
  if ('>' == *p) {
    if (p[1] == '>') return new TokenNode(line, p, 2, Punctuator);        // >>
    if (p[1] == '=') return new TokenNode(line, p, 2, Punctuator);        // >=
    return new TokenNode(line, p, 1, Punctuator);
  }
  if ('-' == *p) {
    if (p[1] == '>') return new TokenNode(line, p, 2, Punctuator);        // ->
    return new TokenNode(line, p, 1, Punctuator);                         // -
  }
  if ('\n' == *p) {
    line++;
    return new TokenNode(line, p, 1, Punctuator);
  }
  if ('=' == *p ||
      '^' == *p || '~' == *p ||
      '+' == *p ||
      '/' == *p || '*' == *p || '%' == *p ||
      '(' == *p || ')' == *p ||
      '[' == *p || ']' == *p ||
      ',' == *p) {
    return new TokenNode(line, p, 1, Punctuator);
  }
  return new TokenNode(line, p, 1, Unknown);
}

void print_tokens(TokenNode *head) {
  for (TokenNode *next = head; next != NULL; next = next->next) {
    if (*next->begin == '\n')
      fprintf(stdout, "code: LF ");
    else
      fprintf(stdout, "code: %.*s ", next->length, next->begin);
    switch (next->type)
    {
      case Delimiter:
        fprintf(stdout, "type: Delimiter\n");
        break;
      case Punctuator:
        fprintf(stdout, "type: Punctuator\n");
        break;
      case NumberConstant:
        fprintf(stdout, "type: NumberConstant\n");
        break;
      case StringLiteral:
        fprintf(stdout, "type: StringLiteral\n");
        break;
      case Ident:
        fprintf(stdout, "type: Ident\n");
        break;
      case KwBreak:
        fprintf(stdout, "type: KwBreak\n");
        break;
      case KwContinue:
        fprintf(stdout, "type: KwContinue\n");
        break;
      case KwElif:
        fprintf(stdout, "type: KwElif\n");
        break;
      case KwElse:
        fprintf(stdout, "type: KwElse\n");
        break;
      case KwFor:
        fprintf(stdout, "type: KwFor\n");
        break;
      case KwFunc:
        fprintf(stdout, "type: KwFunc\n");
        break;
      case KwIf:
        fprintf(stdout, "type: KwIf\n");
        break;
      case KwLoop:
        fprintf(stdout, "type: KwLoop\n");
        break;
      case KwNum:
        fprintf(stdout, "type: KwNum\n");
        break;
      case KwReturn:
        fprintf(stdout, "type: KwReturn\n");
        break;
      case KwStr:
        fprintf(stdout, "type: KwStr\n");
        break;
      case KwVoid:
        fprintf(stdout, "type: KwVoid\n");
        break;
      case Unknown:
        fprintf(stdout, "type: Unknown\n");
        break;
      default:
        break;
    }
  }
}

TokenNode *tokenize(string &source) {
  TokenNode *head = NULL;
  TokenNode **next = &head;
  int l = 1;
  char *p = &source[0];
  for (TokenNode *t=create_next_token(p, l);t!=NULL;) {
    *next = t;
    next = &t->next;
    p = t->begin + t->length;
    t = create_next_token(p, l);
  }
  return head;
}