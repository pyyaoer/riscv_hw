#include "include/inst.h"

static const enum OpType kOpDecodeArr[4][8] = {
  { LOAD, LOAD_FP, ERROR, MISC_MEM,
    OP_IMM, AUIPC, OP_IMM_32, ERROR },
  { STORE, STORE_FP, ERROR, AMO,
    OP, LUI, OP_32, ERROR },
  { MADD, MSUB, NMSUB, NMADD,
    OP_FP, ERROR, ERROR, ERROR },
  { BRANCH, JALR, ERROR, JAL,
    SYSTEM, ERROR, ERROR, ERROR }
};

void Inst::AnalyzeInst() {
  args_.inst = inst_;

  union {
    struct {
      uint32_t op0_1:2, op2_4:3, op5_6:2, rest:25;
    } s_op;
    uint32_t u_op;
  } op_field;
  op_field.u_op = args_.r_type.op;

  if (op_field.s_op.op0_1 != 0x3 || type_ == ERROR) {
    throw "Exception: Unexpected instruction opcode";
  }
  type_ = kOpDecodeArr[op_field.s_op.op5_6][op_field.s_op.op2_4];

  switch (type_) {
    //R-TYPE
    case OP:
    case OP_32:
    case AMO:
    case STORE_FP:
    case OP_FP:
    case MADD:
    case MSUB:
    case NMADD:
    case NMSUB:
      form_ = R_TYPE;
      break;
    //I-TYPE
    case OP_IMM:
    case JALR:
    case LOAD:
    case MISC_MEM:
    case SYSTEM:
    case OP_IMM_32:
    case LOAD_FP:
      form_ = I_TYPE;
      break;
    //S-TYPE
    case STORE:
      args_.s_type.imm = args_.s_type.imm0_4
                  + (args_.s_type.imm5_11 << 5);
      form_ = S_TYPE;
      break;
    //SB-TYPE
    case BRANCH:
      args_.sb_type.imm = (args_.sb_type.imm11 << 11)
                  + (args_.sb_type.imm1_4 << 1)
                  + (args_.sb_type.imm5_10 << 5)
                  + (args_.sb_type.imm12 << 12);
      form_ = SB_TYPE;
      break;
    //U-TYPE
    case LUI:
    case AUIPC:
      args_.u_type.imm = (args_.u_type.imm12_31 << 12);
      form_ = U_TYPE;
      break;
    //UJ-TYPE
    case JAL:
      args_.uj_type.imm = (args_.uj_type.imm12_19 << 12)
                  + (args_.uj_type.imm11 << 11)
                  + (args_.uj_type.imm1_10 << 1)
                  + (args_.uj_type.imm20 << 20);
      form_ = UJ_TYPE;
      break;
    default:
      throw "Exception: Unexpected OpType";
  }
}

