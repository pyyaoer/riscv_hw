#ifndef RISCV_INST_H_
#define RISCV_INST_H_

#include <string>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&); \
  void operator=(const TypeName&)

enum OpType { LOAD, LOAD_FP, MISC_MEM, OP_IMM, AUIPC,
               OP_IMM_32, STORE, STORE_FP, AMO, OP, LUI,
               OP_32, MADD, MSUB, NMSUB, NMADD, OP_FP,
               BRANCH, JALR, JAL, SYSTEM, ERROR };
enum InstFormat { R_TYPE, I_TYPE, S_TYPE, SB_TYPE,
                  U_TYPE, UJ_TYPE };

union InstArgs {
  struct {
    uint64_t op, rd, rs1, func3, rs2, func7;
  } r_type;
  struct {
    uint64_t op, rd, rs1, func3, imm;
  } i_type;
  struct {
    uint64_t op, imm, rs1, func3, rs2;
  } s_type;
  struct {
    uint64_t op, rd, imm;
  } u_type;
};

class Inst {
 public:
  Inst() { Inst(0x13); }
  explicit Inst(uint32_t _i) {
    SetInst(_i);
  }
  ~Inst() {};
  void SetInst(uint32_t _i) {
    inst_ = _i;
    AnalyzeInst();
  }
  void GetArgs(union InstArgs &args,
                enum InstFormat &form) const {
    form = form_;
    memcpy(&args, &args_, sizeof(union InstArgs));
  };
  void GetOpType(enum OpType &type) const {
    type = type_;
  }

 private:
  void AnalyzeInst();

  uint32_t inst_;
  enum OpType type_;
  enum InstFormat form_;
  union InstArgs args_;
  DISALLOW_COPY_AND_ASSIGN(Inst);
};

#endif //RISCV_INST_H_
