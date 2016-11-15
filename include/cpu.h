#ifndef RISCV_CPU_H_
#define RISCV_CPU_H_

#include <fcntl.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "include/vmemory.h"
#include "include/inst.h"

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&); \
  void operator=(const TypeName&)


class CPU {
 public:
  CPU() { CPU(0); };
  explicit CPU(uint64_t pc);
  ~CPU() {
    delete inst_;
  };

  void SetPC(uint64_t pc) { pc_ = pc; }
  void GetPC(uint64_t &pc) const { pc = pc_; };
  void SetReg(uint64_t content, uint32_t n);
  void GetReg(uint64_t &content, uint32_t n) const;
  void SetMem(VMemory* vm) {
    if (vm_ == NULL) vm_ = vm;
    else throw "Exception: VM exists";
  };

  void Fetch();
  void Decode();
  void Execute();

 private:
  // R-TYPE
  void exec_OP(r_type r);
  void exec_OP_32(r_type r);
  void exec_AMO(r_type r);
  void exec_STORE_FP(r_type r);
  void exec_OP_FP(r_type r);
  void exec_MADD(r_type r);
  void exec_MSUB(r_type r);
  void exec_NMADD(r_type r);
  void exec_NMSUB(r_type r);
  // I-TYPE
  void exec_OP_IMM(i_type i);
  void exec_JALR(i_type i);
  void exec_LOAD(i_type i);
  void exec_MISC_MEM(i_type i);
  void exec_SYSTEM(i_type i);
  void exec_OP_IMM_32(i_type i);
  void exec_LOAD_FP(i_type i);
  // S-TYPE
  void exec_STORE(s_type s);
  // SB-TYPE
  void exec_BRANCH(sb_type sb);
  // U-TYPE
  void exec_LUI(u_type u);
  void exec_AUIPC(u_type u);
  // UJ-TYPE
  void exec_JAL(uj_type uj);

  uint64_t pc_;
  uint64_t reg[32];
  Inst* inst_;
  VMemory* vm_;
  DISALLOW_COPY_AND_ASSIGN(CPU);
};


#endif //RISCV_CPU_H_
