#ifndef GENERATOR_HPP
#define GENERATOR_HPP
#include "bits/stdc++.h"

namespace generator {
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

  class SymbolEntrys {
    public:
    SymbolEntrys() {}
    void add_global_var(std::string key, std::shared_ptr<EvalType> type) {}
    void add_func(std::string key, std::shared_ptr<EvalType> type, std::vector<std::shared_ptr<EvalType>> &type_args) {}
    void add_local_var(std::string key, std::shared_ptr<EvalType> type) {}
    void start_scope() {}
    void end_scope() {}
  };
}
#endif