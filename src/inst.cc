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

inline static uint32_t uint_get_a2b(uint32_t inst,
                                     int a, int b) {
  return ((b==31) ? inst : inst & ((1 << (b+1)) - 1)) >> a;
}

void Inst::AnalyzeInst() {
  args_.r_type.op = uint_get_a2b(inst_, 0, 6);
  args_.r_type.rd = uint_get_a2b(inst_, 7, 11);
  args_.r_type.func3 = uint_get_a2b(inst_, 12, 14);
  args_.r_type.rs1 = uint_get_a2b(inst_, 15, 19);
  args_.r_type.rs2 = uint_get_a2b(inst_, 20, 24);
  args_.r_type.func7 = uint_get_a2b(inst_, 25, 31);

  uint32_t op_0_1 = uint_get_a2b(args_.r_type.op, 0, 1);
  uint32_t op_2_4 = uint_get_a2b(args_.r_type.op, 2, 4);
  uint32_t op_5_6 = uint_get_a2b(args_.r_type.op, 5, 6);

  type_ = kOpDecodeArr[op_5_6][op_2_4];
  if (op_0_1 != 0x3 || type_ == ERROR) {
    throw "Unexpected instruction opcode";
  }

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
      args_.i_type.imm = uint_get_a2b(inst_, 20, 31);
      form_ = I_TYPE;
      break;
    //S-TYPE
    case STORE:
      args_.s_type.imm = uint_get_a2b(inst_, 7, 11)
                  + ((uint_get_a2b(inst_, 25, 31)) << 5);
      form_ = S_TYPE;
      break;
    //SB-TYPE
    case BRANCH:
      args_.s_type.imm = (uint_get_a2b(inst_, 8, 11) << 1)
                  + ((uint_get_a2b(inst_, 25, 30)) << 5)
                  + ((uint_get_a2b(inst_, 7, 7)) << 11)
                  + ((uint_get_a2b(inst_, 31, 31)) << 12);
      form_ = SB_TYPE;
      break;
    //U-TYPE
    case LUI:
    case AUIPC:
      args_.u_type.imm = (uint_get_a2b(inst_, 12, 31) << 12);
      form_ = U_TYPE;
      break;
    //UJ-TYPE
    case JAL:
      args_.u_type.imm = (uint_get_a2b(inst_, 21, 30) << 1)
                  + ((uint_get_a2b(inst_, 20, 20)) << 11)
                  + ((uint_get_a2b(inst_, 12, 19)) << 12)
                  + ((uint_get_a2b(inst_, 31, 31)) << 20);
      form_ = UJ_TYPE;
      break;
    default:
      throw "Exception: Unexpected OpType";
  }
}

