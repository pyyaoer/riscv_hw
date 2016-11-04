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

static void op_load();
static void op_error();

void Inst::AnalyzeInst() {
  decode_.op = uint_get_a2b(inst_, 0, 6);
  decode_.rd = uint_get_a2b(inst_, 7, 11);
  decode_.rs1 = uint_get_a2b(inst_, 15, 19);
  decode_.rs2 = uint_get_a2b(inst_, 20, 24);
  decode_.func3 = uint_get_a2b(inst_, 12, 14);
  decode_.func7 = uint_get_a2b(inst_, 25, 31);
  uint32_t op_0_1 = uint_get_a2b(decode_.op, 0, 1);
  uint32_t op_2_4 = uint_get_a2b(decode_.op, 2, 4);
  uint32_t op_5_6 = uint_get_a2b(decode_.op, 5, 6);

  enum OpType inst_type = kOpDecodeArr[op_5_6][op_2_4];
  if (op_0_1 != 0x3 || inst_type == ERROR) {
    throw "Unexpected instruction opcode";
  }

  switch (inst_type) {
    case LOAD:
      op_load();
      break;
    case LOAD_FP:
    case MISC_MEM:
    case OP_IMM:
    case AUIPC:
    case OP_IMM_32:
    case STORE:
    case STORE_FP:
    case AMO:
    case OP:
    case LUI:
    case OP_32:
    case MADD:
    case MSUB:
    case NMSUB:
    case NMADD:
    case OP_FP:
    case BRANCH:
    case JALR:
    case JAL:
    case SYSTEM:
    default:
      throw "Unsupported instruction";
  }
}

static void op_load() {
  return;
}
