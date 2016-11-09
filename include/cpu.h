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
  //void IsValidRegIndex(uint32_t n) const;

  uint64_t pc_;
  uint64_t reg[32];
  Inst* inst_;
  VMemory* vm_;
  DISALLOW_COPY_AND_ASSIGN(CPU);
};


#endif //RISCV_CPU_H_
