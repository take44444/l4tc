#include "lupc.h"

TokenNode *expect_token_with_str(TokenNode **next, Error &err, string str) {
  if (!(*next)->is_equal_with_str(&str[0])) {
    err = Error("expect " + str);
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
  if ((*next)->type != type) {
    err = Error("error");
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