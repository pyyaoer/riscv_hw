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

typedef struct {
  uint32_t op:7, rd:5, func3:3, rs1:5, rs2:5, func7:7;
} r_type;
typedef struct {
  uint32_t op:7, rd:5, func3:3, rs1:5, imm:12;
} i_type;
typedef struct {
  uint32_t op:7, imm0_4:5, func3:3, rs1:5, rs2:5, imm5_11:7;
  uint32_t imm;
} s_type;
typedef struct {
  uint32_t op:7, imm11:1, imm1_4:4, func3:3;
  uint32_t rs1:5, rs2:5, imm5_10:6, imm12:1;
  uint32_t imm;
} sb_type;
typedef struct {
  uint32_t op:7, rd:5, imm12_31:20;
  uint32_t imm;
} u_type;
typedef struct {
  uint32_t op:7, rd:5, imm12_19:8, imm11:1;
  uint32_t imm1_10:10, imm20:1;
  uint32_t imm;
} uj_type;

union InstArgs {
  r_type r;
  i_type i;
  s_type s;
  sb_type sb;
  u_type u;
  uj_type uj;
  uint32_t inst;
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
