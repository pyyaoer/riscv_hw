#ifndef RISCV_INST_H_
#define RISCV_INST_H_

#include <string>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&); \
  void operator=(const TypeName&)

enum OpType { LOADS, STORES, F_EXT, BRANCHES, JALR,
               FENCES, A_EXT, JAL, ARITHMETIC, SYSTEM,
               AUIPC, LUI, RV64I, RV128I, ERROR };

class Inst {
 public:
  Inst() { Inst(0); }
  explicit Inst(uint32_t _i) {
    SetInst(_i);
  }
  ~Inst() {};
  void SetInst(uint32_t _i) {
    inst_ = _i;
    AnalyzeInst();
  }

 private:
  void AnalyzeInst();

  uint32_t inst_;
  struct {
    uint32_t op;
    uint32_t rd;
    uint32_t rs1;
    uint32_t rs2;
    uint32_t func3;
    uint32_t func7;
    uint32_t imm;
  } decode_;
  DISALLOW_COPY_AND_ASSIGN(Inst);
};

#endif //RISCV_INST_H_
