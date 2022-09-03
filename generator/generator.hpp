#ifndef GENERATOR_HPP
#define GENERATOR_HPP
#include "bits/stdc++.h"
#include "../tokenizer/tokenizer.hpp"
#include "../parser/parser.hpp"

namespace generator {
  using namespace tokenizer;
  using namespace parser;
  const std::string param_reg_names[6] = {"rdi", "rsi", "rdx", "rcx", "r8",  "r9"};

  class EvalType {
    public:
    int size;
    EvalType() {}
    virtual ~EvalType() = default;
  };

  class TypeNum : public EvalType {
    public:
    TypeNum() : EvalType() {
      size = 8;
    }
  };

  class TypePtr : public EvalType {
    public:
    std::shared_ptr<EvalType> of;
    TypePtr(std::shared_ptr<EvalType> of) : EvalType(), of(of) {
      size = 8;
    }
  };

  class TypeChar : public EvalType {
    public:
    TypeChar() : EvalType() {
      size = 1;
    }
  };

  class TypeArray : public EvalType {
    public:
    std::shared_ptr<EvalType> of;
    TypeArray(std::shared_ptr<EvalType> of, int sz) : EvalType(), of(of) {
      size = sz * of->size;
    }
  };

  class TypeFunc : public EvalType {
    public:
    std::vector<std::shared_ptr<EvalType>> type_args;
    std::shared_ptr<EvalType> ret_type;
    explicit TypeFunc(std::vector<std::shared_ptr<EvalType>> &ta, std::shared_ptr<EvalType> rt)
    : EvalType(), type_args(ta), ret_type(rt) {
      size = 8;
    }
    explicit TypeFunc(std::shared_ptr<EvalType> rt)
    : EvalType(), ret_type(rt) {
      size = 8;
    }
  };

  class TypeAny : public EvalType {
    public:
    TypeAny() : EvalType() {}
  };

  // class LoopInfo {
  //   public:
  //   std::string label_break;
  //   std::string label_continue;
  //   LoopInfo(int i) {
  //     if (i < 0) {
  //       label_break = "";
  //       label_continue = "";
  //       return;
  //     }
  //     label_break = std::to_string(i) + "B";
  //     label_continue = std::to_string(i) + "C";
  //   }
  // };

  class GlobalVar {
    public:
    std::string name;
    std::shared_ptr<EvalType> type;
    GlobalVar(std::string n, std::shared_ptr<EvalType> t) : name(n), type(t) {}
  };

  class Func {
    public:
    std::string name;
    std::shared_ptr<EvalType> type;
    Func(std::string n, std::shared_ptr<EvalType> t) : name(n), type(t) {}
  };

  class LocalVar {
    public:
    int offset;
    std::shared_ptr<EvalType> type;
    LocalVar(int o, std::shared_ptr<EvalType> t) : offset(o), type(t) {}
  };

  class Context {
    public:
    int rsp;
    // std::vector<int> saved_rsp;
    std::vector<std::map<std::string, std::shared_ptr<LocalVar>>> scopes_local_vars;
    std::map<std::string, std::shared_ptr<GlobalVar>> global_vars;
    std::map<std::string, std::shared_ptr<Func>> funcs;
    std::vector<std::pair<std::string, std::string>> strs;

    Context() : rsp(0) {}

    void add_func(std::string key, std::shared_ptr<EvalType> type) {
      funcs.insert({key, std::make_shared<Func>(key, type)});
    }

    void add_global_var(std::string key, std::shared_ptr<EvalType> type) {
      global_vars.insert({key, std::make_shared<GlobalVar>(key, type)});
    }

    void add_local_var(std::string key, std::shared_ptr<EvalType> type) {
      scopes_local_vars.back().insert(
        {key, std::make_shared<LocalVar>(-rsp, type)}
      );
    }

    void add_str(std::string label, std::string_view data) {
      strs.push_back({label, std::string(data)});
    }

    std::shared_ptr<Func> get_func(std::string_view key) {
      auto it = funcs.find(std::string(key));
      if (it == funcs.end()) return nullptr;
      return it->second;
    }

    std::shared_ptr<GlobalVar> get_global_var(std::string_view key) {
      auto it = global_vars.find(std::string(key));
      if (it == global_vars.end()) return nullptr;
      return it->second;
    }

    std::shared_ptr<LocalVar> get_local_var(std::string_view key) {
      for (int i=(int)scopes_local_vars.size()-1; i >= 0; i--) {
        auto it = scopes_local_vars[i].find(std::string(key));
        if (it == scopes_local_vars[i].end()) continue;
        return it->second;
      }
      return nullptr;
    }

    void start_scope() {
      // saved_rsp.push_back(rsp);
      scopes_local_vars.push_back(
        std::map<std::string, std::shared_ptr<LocalVar>>()
      );
    }

    void end_scope() {
      // assert(saved_rsp.back() == rsp);
      // saved_rsp.pop_back();
      scopes_local_vars.pop_back();
    }

    // LoopInfo get_loop() {
    //   return LoopInfo(-1);
    // }

    // bool is_rsp_aligned() {
    //   std::cerr << rsp << std::endl;
    //   return !(rsp & 0xF);
    // }
  };
  int string_literal_length(std::string_view sl);
  bool is_aligned_16(int x);
  int align_8(int x);
  void eval(std::shared_ptr<ASTExpr> expr, std::string reg, std::string &code);
  std::string create_label();
  void derefer(std::string reg, std::string &code);
  void get_func_addr(std::string reg, std::shared_ptr<Func> f, std::string &code);
  void get_global_var_addr(std::string reg, std::shared_ptr<GlobalVar> gv, std::string &code);
  void get_str_addr(std::string reg, std::string &label, std::string &code);
  void get_local_var_addr(std::string reg, std::shared_ptr<LocalVar> lv, std::string &code);
  void lea_label(std::string reg, std::string label, std::string &code);
  void push(std::string reg, std::shared_ptr<Context> ctx, std::string &code);
  void pop(std::string reg, std::shared_ptr<Context> ctx, std::string &code);
  void call(std::string reg, std::shared_ptr<Context> ctx, std::string &code);
  void ret(std::string &code);
  bool type_eq(std::shared_ptr<EvalType> x, std::shared_ptr<EvalType> y);
  bool try_assign(std::shared_ptr<ASTExpr> l, std::shared_ptr<ASTExpr> r,
                  std::shared_ptr<Context> ctx, std::string &code);
  void push_args(int i, std::shared_ptr<EvalType> type, std::shared_ptr<Context> ctx, std::string &code);
  std::shared_ptr<EvalType> create_type(std::shared_ptr<ASTTypeSpec> n);
  std::shared_ptr<TypeFunc> create_func_type(std::shared_ptr<ASTFuncDeclaration> fd);
  std::string generate(std::shared_ptr<AST> ast);
}
#endif