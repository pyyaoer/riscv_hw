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
  op_field.u_op = args_.r.op;

  if (op_field.s_op.op0_1 != 0x3 || type_ == ERROR) {
    throw "Exception: Unexpected instruction opcode";
  }
  type_ = kOpDecodeArr[op_field.s_op.op5_6][op_field.s_op.op2_4];

  s_type s = args_.s;
  sb_type sb = args_.sb;
  u_type u = args_.u;
  uj_type uj = args_.uj;

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
      s.imm = s.imm0_4 + (s.imm5_11 << 5);
      form_ = S_TYPE;
      break;
    //SB-TYPE
    case BRANCH:
      sb.imm = (sb.imm11 << 11)
             + (sb.imm1_4 << 1)
             + (sb.imm5_10 << 5)
             + (sb.imm12 << 12);
      form_ = SB_TYPE;
      break;
    //U-TYPE
    case LUI:
    case AUIPC:
      u.imm = (u.imm12_31 << 12);
      form_ = U_TYPE;
      break;
    //UJ-TYPE
    case JAL:
      uj.imm = (uj.imm12_19 << 12)
             + (uj.imm11 << 11)
             + (uj.imm1_10 << 1)
             + (uj.imm20 << 20);
      form_ = UJ_TYPE;
      break;
    default:
      throw "Exception: Unexpected OpType";
  }
}

