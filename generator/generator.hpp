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

  class TypeStruct : public EvalType {
    public:
    std::string name;
    std::map<std::string, std::pair<int, std::shared_ptr<EvalType>>> members;
    TypeStruct(std::string_view n) : EvalType(), name(n) {
      size = 0;
    }
    void add_member(std::string_view key, std::shared_ptr<EvalType> type) {
      members.insert({std::string(key), {size, type}});
      size += type->size;
    }
    std::pair<int, std::shared_ptr<EvalType>> get_member(std::string_view key) {
      auto it = members.find(std::string(key));
      if (it == members.end()) return {0, nullptr};
      return it->second;
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
    std::shared_ptr<TypeFunc> type;
    Func(std::string n, std::shared_ptr<TypeFunc> t) : name(n), type(t) {}
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
    std::map<std::string, std::shared_ptr<TypeStruct>> structs;

    Context() : rsp(0) {}

    void add_func(std::string key, std::shared_ptr<TypeFunc> type) {
      funcs.insert({key, std::make_shared<Func>(key, type)});
    }

    void add_struct(std::string_view key, std::shared_ptr<TypeStruct> type) {
      structs.insert({std::string(key), type});
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

    std::shared_ptr<TypeStruct> get_struct(std::string key) {
      auto it = structs.find(key);
      if (it == structs.end()) return nullptr;
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
    std::shared_ptr<EvalType> create_type(std::shared_ptr<ASTTypeSpec> n) {
      std::vector<std::shared_ptr<EvalType>> type_args;
      std::shared_ptr<EvalType> type = nullptr;
      switch (n->op->type)
      {
      case KwNum:
        return std::make_shared<TypeNum>();
      case KwArray:
        if (!(type = create_type(n->of))) return nullptr;
        return std::make_shared<TypeArray>(type, n->size);
      case KwChar:
        return std::make_shared<TypeChar>();
      case KwPtr:
        if (!(type = create_type(n->of))) return nullptr;
        return std::make_shared<TypePtr>(type);
      case KwFuncptr:
        for (int i = 0; i < (int)n->args.size(); i++) {
          if (!(type = create_type(n->args[i]))) return nullptr;
          type_args.push_back(type);
        }
        if (!(type = create_type(n->type_spec))) return nullptr;
        return std::make_shared<TypeFunc>(type_args, type);
      case KwStruct:
        type = get_struct(n->s_name);
        if (!type) {
          assert(false);
        }
        return type;
      default:
        break;
      }
      assert(false);
      return nullptr;
    }

    std::shared_ptr<TypeFunc> create_func_type(std::shared_ptr<ASTFuncDeclaration> fd) {
      std::vector<std::shared_ptr<EvalType>> type_args;
      std::shared_ptr<EvalType> type = nullptr;
      for (std::shared_ptr<ASTSimpleDeclaration> d: fd->declarator->args) {
        if (!(type = create_type(d->type_spec))) return nullptr;
        type_args.push_back(type);
      }
      if (!(type = create_type(fd->type_spec))) return nullptr;
      return std::make_shared<TypeFunc>(type_args, type);
    }

    std::shared_ptr<TypeStruct> create_struct_type(std::shared_ptr<ASTStructDef> sd) {
      std::shared_ptr<TypeStruct> ret = std::make_shared<TypeStruct>(
        sd->declarator->op->sv
      );
      std::shared_ptr<EvalType> type = nullptr;
      for (std::shared_ptr<ASTSimpleDeclaration> d: sd->declarations) {
        if (!(type = create_type(d->type_spec))) return nullptr;
        ret->add_member(d->declarator->op->sv, type);
      }
      return ret;
    }
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
  std::string generate(std::shared_ptr<AST> ast);
}
#endif