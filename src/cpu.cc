#include <string.h>
#include "include/cpu.h"

int64_t u64_to_i64(uint64_t src, int sign_bit_pos) {
  union {
    uint64_t u;
    int64_t i;
  } u2i;
  u2i.u = src;
  if (src & (1 << (sign_bit_pos - 1))){
    u2i.u |= ~(u2i.u | ((1 << sign_bit_pos) - 1));
  }
  return u2i.i;
}

inline static void IsValidRegIndex(uint32_t n) {
  if (n >= 32) {
    throw "Exception: Unexpected Register Access";
  }
}

CPU::CPU(uint64_t pc) {
  pc_ = pc;
  memset(reg, 0, sizeof(reg));
  inst_ = new Inst;
  vm_ = NULL;
}

void CPU::SetReg(uint64_t content, uint32_t n) {
  IsValidRegIndex(n);
  reg[n] = content;
}

void CPU::GetReg(uint64_t &content, uint32_t n) const {
  IsValidRegIndex(n);
  content = reg[n];
}

void CPU::Fetch() {
  union {
    char c_content[4];
    uint32_t i_content;
  } content;
  if (vm_ == NULL) throw "Exception: Memory 404";
  vm_->GetNByte(pc_, content.c_content, 4);
  inst_->SetInst(content.i_content);
}

void CPU::Decode() {
  enum OpType type;
  union InstArgs args;
  enum InstFormat form;
  inst_->GetOpType(type);
  inst_->GetArgs(args, form);
  switch (type) {
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
      break;
    //I-TYPE
    case OP_IMM:
      uint64_t rd_content;
      uint64_t rs1_content;
      int64_t imm_signed;
      GetReg(rs1_content, args.i_type.rd);
      imm_signed = u64_to_i64(args.i_type.imm, 12);
      switch (args.i_type.func3) {
        case 0x0: // ADDI
          rd_content = rs1_content + imm_signed;
        case 0x2: // SLTI
          rd_content = static_cast<int64_t>(rs1_content)
                     < imm_signed ? 1 : 0;
        case 0x3: // SLTIU
          rd_content = static_cast<uint64_t>(imm_signed)
                     > rs1_content ? 0 : 1;
        case 0x4: // XORI
          rd_content = rs1_content ^ imm_signed;
        case 0x6: // ORI
          rd_content = rs1_content | imm_signed;
        case 0x7: // ANDI
          rd_content = rs1_content & imm_signed;
          break;
        default:
          throw "Exception: Unexpected func3";
      }
      SetReg(rs1_content, args.i_type.rs1);
      break;
    case JALR:
    case LOAD:
    case MISC_MEM:
    case SYSTEM:
    case OP_IMM_32:
    case LOAD_FP:
      break;
    //S-TYPE
    case STORE:
      break;
    //SB-TYPE
    case BRANCH:
      break;
    //U-TYPE
    case LUI:
    case AUIPC:
      break;
    //UJ-TYPE
    case JAL:
      break;
  }
}

void CPU::Execute() {
  throw "Program terminated successfully";
}

