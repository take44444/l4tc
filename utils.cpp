#include "lupc.h"

TokenNode *expect_token_with_str(TokenNode **next, Error &err, string str) {
  // NULL check
  if ((*next) == NULL) {
    err = Error("expected ????, found EOF");
    return NULL;
  }
  if (!(*next)->is_equal_with_str(&str[0])) {
    err = Error("expected ????, found ????");
    return NULL;
  }
  TokenNode *ret = *next;
  *next = (*next)->next;
  return ret;
}

TokenNode *consume_token_with_str(TokenNode **next, string str) {
  if (!(*next)->is_equal_with_str(&str[0])) {
    return NULL;
  }
  TokenNode *ret = *next;
  *next = (*next)->next;
  return ret;
}

TokenNode *expect_token_with_type(TokenNode **next, Error &err, TokenType type) {
  // NULL check
  if ((*next) == NULL) {
    err = Error("expected ????, found EOF");
    return NULL;
  }
  if ((*next)->type != type) {
    err = Error("expected ????, found ????");
    return NULL;
  }
  TokenNode *ret = *next;
  *next = (*next)->next;
  return ret;
}

TokenNode *consume_token_with_type(TokenNode **next, TokenType type) {
  if ((*next)->type != type) {
    return NULL;
  }
  TokenNode *ret = *next;
  *next = (*next)->next;
  return ret;
}

TokenNode *consume_token_with_indents(TokenNode **next, int indents) {
  assert(*((*next)->begin) == ' ');
  if ((*next)->length != indents) {
    return NULL;
  }
  TokenNode *ret = *next;
  *next = (*next)->next;
  return ret;
}