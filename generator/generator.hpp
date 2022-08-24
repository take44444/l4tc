#ifndef GENERATOR_HPP
#define GENERATOR_HPP
#include "bits/stdc++.h"
#include "../tokenizer/tokenizer.hpp"
#include "../parser/parser.hpp"

namespace generator {
  using namespace tokenizer;
  using namespace parser;
  class EvalType {
    public:
    EvalType() {}
  };

  class TypeNum : public EvalType {
    public:
    TypeNum() : EvalType() {}
  };

  class TypeVoid : public EvalType {
    public:
    TypeVoid() : EvalType() {}
  };

  class TypePointer : public EvalType {
    public:
    std::shared_ptr<EvalType> pointer_of;
    TypePointer(std::shared_ptr<EvalType> of) : EvalType(), pointer_of(of) {}
  };

  class LoopInfo {
    public:
    std::string label_break;
    std::string label_continue;
    LoopInfo(int i) {
      if (i < 0) {
        label_break = "";
        label_continue = "";
        return;
      }
      label_break = std::to_string(i) + "B";
      label_continue = std::to_string(i) + "C";
    }
  };

  class SymbolEntrys {
    public:
    int rsp;
    SymbolEntrys() : rsp(0) {}
    void add_global_var(std::string key, std::shared_ptr<EvalType> type) {}
    void add_func(std::string key, std::shared_ptr<EvalType> type, std::vector<std::shared_ptr<EvalType>> &type_args) {}
    void add_local_var(std::string key, std::shared_ptr<EvalType> type) {}
    void start_scope() {}
    void end_scope() {}
    LoopInfo get_loop_info() {
      return LoopInfo(-1);
    }
    void add2rsp(int o) {
      rsp += o;
    }
    bool is_rsp_aligned() {
      return !(rsp & 0xF);
    }
  };
}
#endif