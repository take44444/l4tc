#include "lupc.h"

Token *expect_token_with_str(Token **next, Error &err, string str) {
  // NULL check
  if ((*next) == NULL) {
    err = Error("expected ????, found EOF");
    return NULL;
  }
  if (!(*next)->is_equal_with_str(&str[0])) {
    err = Error("expected ????, found ????");
    return NULL;
  }
  Token *ret = *next;
  *next = (*next)->next;
  return ret;
}

Token *consume_token_with_str(Token **next, string str) {
  if (!(*next)->is_equal_with_str(&str[0])) return NULL;
  Token *ret = *next;
  *next = (*next)->next;
  return ret;
}

Token *expect_token_with_type(Token **next, Error &err, TokenType type) {
  // NULL check
  if ((*next) == NULL) {
    err = Error("expected ????, found EOF");
    return NULL;
  }
  if ((*next)->type != type) {
    err = Error("expected ????, found ????");
    return NULL;
  }
  Token *ret = *next;
  *next = (*next)->next;
  return ret;
}

Token *consume_token_with_type(Token **next, TokenType type) {
  if ((*next)->type != type) return NULL;
  Token *ret = *next;
  *next = (*next)->next;
  return ret;
}

Token *consume_token_with_indents(Token **next, int indents) {
  if (*((*next)->begin) != ' ' || (*next)->length != indents) return NULL;
  Token *ret = *next;
  *next = (*next)->next;
  return ret;
}