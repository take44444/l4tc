#include "./generator.hpp"

namespace generator {
  int string_literal_length(std::string_view sl) {
    int ret = sl.length() - 2 + 1;
    for (char c: sl) if (c == '\\') ret--;
    return ret;
  }

  bool is_aligned_16(int x) {
    return !(x & 0xF);
  }

  int align_8(int x) {
    return ((x + 7) >> 3) << 3;
  }

  void eval(std::shared_ptr<ASTExpr> expr, std::string reg, std::string &code) {
    if (expr->is_assignable) code += "mov " + reg + ", [" + reg + "]\n";
  }

  std::string create_label() {
    static int label_number = 0;
    return "L" + std::to_string(label_number++);
  }

  void cmp(std::string reg, std::string l, std::string r,
           std::string op, std::string &code) {
    code += "cmp " + l + ", " + r +"\n";
    code += "set" + op + " " + reg +"b\n";
    code += "movzx " + reg + ", " + reg +"b\n";
  }

  void derefer(std::string reg, std::string &code) {
    code += "mov " + reg + ", [" + reg +"]\n";
  }

  void get_func_addr(std::string reg, std::shared_ptr<Func> f, std::string &code) {
    code += "mov " + reg + ", [rip + " + f->name + "@GOTPCREL]\n";
  }

  void get_ffi_addr(std::string reg, std::shared_ptr<Ffi> f, std::string &code) {
    code += "mov " + reg + ", [rip + " + f->name + "@GOTPCREL]\n";
  }

  void get_global_var_addr(std::string reg, std::shared_ptr<GlobalVar> gv, std::string &code) {
    code += "mov " + reg + ", [rip + " + gv->name + "@GOTPCREL]\n";
  }

  void get_str_addr(std::string reg, std::string &label, std::string &code) {
    code += "lea " + reg + ", [rip + " + label + "]\n";
  }

  void get_local_var_addr(std::string reg, std::shared_ptr<LocalVar> lv, std::string &code) {
    code += "lea " + reg + ", [rbp - " + std::to_string(lv->offset) + "]\n";
  }

  void push(std::string reg, std::shared_ptr<Context> ctx, std::string &code) {
    code += "push " + reg + "\n";
    ctx->rsp -= 8;
  }

  void pop(std::string reg, std::shared_ptr<Context> ctx, std::string &code) {
    code += "pop " + reg + "\n";
    ctx->rsp += 8;
  }

  void call(std::string reg, std::shared_ptr<Context> ctx, std::string &code) {
    if (!is_aligned_16(ctx->rsp)) code += "sub rsp, 8\n";
    code += "call " + reg + "\n";
    if (!is_aligned_16(ctx->rsp)) code += "add rsp, 8\n";
  }

  void ret(std::string &code) {
    code += "mov rsp, rbp\n";
    code += "pop rbp\n";
    code += "ret\n";
  }

  bool type_eq(std::shared_ptr<EvalType> x, std::shared_ptr<EvalType> y) {
    if (typeid(*x) == typeid(TypeFfi) || typeid(*y) == typeid(TypeFfi)) return true;
    if (typeid(*x) != typeid(*y)) return false;
    if (typeid(*x) == typeid(TypeArray)) {
      std::shared_ptr<TypeArray> xx = std::dynamic_pointer_cast<TypeArray>(x);
      std::shared_ptr<TypeArray> yy = std::dynamic_pointer_cast<TypeArray>(y);
      return xx->size == yy->size && type_eq(xx->of, yy->of);
    }
    if (typeid(*x) == typeid(TypePtr)) {
      std::shared_ptr<TypePtr> xx = std::dynamic_pointer_cast<TypePtr>(x);
      std::shared_ptr<TypePtr> yy = std::dynamic_pointer_cast<TypePtr>(y);
      return type_eq(xx->of, yy->of);
    }
    if (typeid(*x) == typeid(TypeFunc)) {
      std::shared_ptr<TypeFunc> xx = std::dynamic_pointer_cast<TypeFunc>(x);
      std::shared_ptr<TypeFunc> yy = std::dynamic_pointer_cast<TypeFunc>(y);
      if (xx->type_args.size() != yy->type_args.size()) return false;
      bool ret = type_eq(xx->ret_type, yy->ret_type);
      for (int i=0; i < (int)xx->type_args.size(); i++) {
        ret = ret && type_eq(xx->type_args[i], yy->type_args[i]);
      }
      return ret;
    }
    if (typeid(*x) == typeid(TypeStruct)) {
      std::shared_ptr<TypeStruct> xx = std::dynamic_pointer_cast<TypeStruct>(x);
      std::shared_ptr<TypeStruct> yy = std::dynamic_pointer_cast<TypeStruct>(y);
      return xx->name == yy->name;
    }
    return true;
  }

  bool try_assign(std::shared_ptr<ASTExpr> l, std::shared_ptr<ASTExpr> r,
                  std::shared_ptr<Context> ctx, std::string &code) {
    if (!l->is_assignable) return false;
    pop("r10", ctx, code);
    pop("r11", ctx, code);
    if (typeid(*l->eval_type) == typeid(TypeArray)) {
      int size = std::dynamic_pointer_cast<TypeArray>(l->eval_type)->size;
      push("r11", ctx, code);
      for (int i = 0; i < align_8(size) / 8; i++) {
        code += "mov r12, [r11]\n";
        code += "mov [r10], r12\n";
        code += "add r10, 8\n";
        code += "add r11, 8\n";
      }
      return true;
    }
    if (typeid(*l->eval_type) == typeid(TypeStruct)) {
      int size = std::dynamic_pointer_cast<TypeStruct>(l->eval_type)->size;
      push("r11", ctx, code);
      for (int i = 0; i < align_8(size) / 8; i++) {
        code += "mov r12, [r11]\n";
        code += "mov [r10], r12\n";
        code += "add r10, 8\n";
        code += "add r11, 8\n";
      }
      return true;
    }
    eval(r, "r11", code);
    code += "mov [r10], r11\n";
    push("r11", ctx, code);
    return true;
  }

  void push_args(int i, std::shared_ptr<EvalType> type, std::shared_ptr<Context> ctx, std::string &code) {
    code += "sub rsp, " + std::to_string(align_8(type->size)) + "\n";
    ctx->rsp -= align_8(type->size);
    if (typeid(*type) == typeid(TypeArray)) { // 値渡し
      code += "lea r10, [rbp - " + std::to_string(-ctx->rsp) + "]\n";
      code += "mov r11, " + param_reg_names[i] + "\n";
      for (int i = 0; i < align_8(type->size) / 8; i++) {
        code += "mov r12, [r11]\n";
        code += "mov [r10], r12\n";
        code += "add r10, 8\n";
        code += "add r11, 8\n";
      }
      return;
    }
    if (typeid(*type) == typeid(TypeStruct)) { // 値渡し
      code += "lea r10, [rbp - " + std::to_string(-ctx->rsp) + "]\n";
      code += "mov r11, " + param_reg_names[i] + "\n";
      for (int i = 0; i < align_8(type->size) / 8; i++) {
        code += "mov r12, [r11]\n";
        code += "mov [r10], r12\n";
        code += "add r10, 8\n";
        code += "add r11, 8\n";
      }
      return;
    }
    code += "mov [rbp - " + std::to_string(-ctx->rsp) + "], " + param_reg_names[i] + "\n";
    return;
  }
  bool dfs(int dfs_id,
           std::string_view node,
           std::map<std::string_view, std::shared_ptr<ASTStructDef>> &nodes,
           std::map<std::string_view, int> &visited,
           std::vector<std::shared_ptr<ASTStructDef>> &ordered_sds,
           GError &err) {
    auto it = visited.find(node);
    std::shared_ptr<ASTStructDef> now = nodes.at(node);
    if (it->second == dfs_id) {
      err.message = "";
      return false;
    }
    if (it->second) return true;
    it->second = dfs_id;
    for (std::shared_ptr<ASTSimpleDeclaration> next: now->declarations) {
      std::shared_ptr<ASTTypeSpec> type_spec = next->type_spec;
      while (type_spec->op->type == KwArray) type_spec = type_spec->of;
      if (type_spec->op->type != KwStruct) continue;
      if (visited.find(type_spec->s->sv) == visited.end()) {
        err = GError(
          std::string(type_spec->s->sv) + " is not defined",
          type_spec->s
        );
        return false;
      }
      if (!dfs(dfs_id,
               type_spec->s->sv,
               nodes,
               visited,
               ordered_sds,
               err)) {
        if (!err.message.size()) {
          err = GError(
            "cannot determine size of struct " +
            std::string(node), type_spec->s
          );
        }
        return false;
      }
    }
    ordered_sds.push_back(now);
    return true;
  }
  std::vector<std::shared_ptr<ASTStructDef>> is_dag(std::vector<std::shared_ptr<ASTStructDef>> &sds, GError &err) {
    std::vector<std::shared_ptr<ASTStructDef>> ret;
    std::map<std::string_view, std::shared_ptr<ASTStructDef>> nodes;
    std::map<std::string_view, int> visited;
    for (std::shared_ptr<ASTStructDef> sd: sds) {
      nodes.insert({sd->declarator->op->sv, sd});
      visited.insert({sd->declarator->op->sv, 0});
    }
    for (int i = 1; i <= (int)sds.size(); i++) {
      if(!dfs(i, sds[i-1]->declarator->op->sv, nodes, visited, ret, err)) return {};
    }
    return ret;
  }
}