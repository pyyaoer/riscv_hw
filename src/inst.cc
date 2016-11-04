#include "include/inst.h"

static const enum OpType kOpDecodeArr[4][8] = {
  { LOADS, F_EXT, ERROR, FENCES,
    ARITHMETIC, AUIPC, RV64I, ERROR },
  { STORES, F_EXT, ERROR, A_EXT,
    ARITHMETIC, LUI, RV64I, ERROR },
  { F_EXT, F_EXT, F_EXT, F_EXT,
    F_EXT, ERROR, RV128I, ERROR },
  { BRANCHES, JALR, ERROR, JAL,
    SYSTEM, ERROR, RV128I, ERROR }
};

inline static uint32_t uint_get_a2b(uint32_t inst,
                                     int a, int b) {
  return ((b==31) ? inst : inst & (1 << (b+1) - 1)) >> a;
}

static void op_loads();
static void op_stores();
static void op_fext();
static void op_branches();
static void op_jalr();
static void op_fences();
static void op_aext();
static void op_jal();
static void op_arithmetic();
static void op_system();
static void op_auipc();
static void op_lui();
static void op_rv64i();
static void op_rv128i();
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
    case LOADS:
      op_loads();
      break;
    case STORES:
    case F_EXT:
    case BRANCHES:
    case JALR:
    case FENCES:
    case A_EXT:
    case JAL:
    case ARITHMETIC:
    case SYSTEM:
    case AUIPC:
    case LUI:
    case RV64I:
    case RV128I:
    default:
      throw "Unsupported instruction";
  }
}

static void op_loads() {
  return;
}
