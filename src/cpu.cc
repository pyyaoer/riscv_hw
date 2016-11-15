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

  r_type r = args.r;
  i_type i = args.i;
  s_type s = args.s;
  sb_type sb = args.sb;
  u_type u = args.u;
  uj_type uj = args.uj;

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
      exec_OP_IMM(i);
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
      exec_LUI(u);
      break;
    case AUIPC:
      exec_AUIPC(u);
      break;
    //UJ-TYPE
    case JAL:
      break;
  }
}

void CPU::Execute() {
  throw "Program terminated successfully";
}


void CPU::exec_OP(r_type r) {
  uint64_t rd_content;
  uint64_t rs1_content;
  GetReg(rs1_content, r.rs1);
  uint64_t rs2_content;
  GetReg(rs2_content, r.rs2);
  switch (r.func3) {
    case 0x0: // ADD | SUB
      if (r.func7 == 0)
        rd_content = rs1_content + rs2_content;
      else
        rd_content = rs1_content - rs2_content;
      break;
    case 0x1: // SLL
      rd_content = rs1_content << (rs2_content & 0x1f);
      break;
    case 0x2: // SLT
      rd_content = (rs1_content < rs2_content) ? 1 : 0;
      break;
    case 0x3: // SLTU
      rd_content = (static_cast<int64_t>(rs1_content)
             < static_cast<int64_t>(rs2_content)) ? 1: 0;
      break;
    case 0x4: // XOR
      rd_content = rs1_content ^ rs2_content;
      break;
    case 0x5: // SRL | SRA
      if (r.func7 == 0)
        rd_content = rs1_content >> (rs2_content & 0x1f);
      else
        rd_content = static_cast<int64_t>(rs1_content)
                     >> (rs2_content & 0x1f);
      break;
    case 0x6: // OR
      rd_content = rs1_content | rs2_content;
      break;
    case 0x7: // AND
      rd_content = rs1_content & rs2_content;
      break;
  }
  SetReg(rd_content, r.rd);
}

void CPU::exec_OP_32(r_type r) {
}

void CPU::exec_AMO(r_type r) {
}

void CPU::exec_STORE_FP(r_type r) {
}

void CPU::exec_OP_FP(r_type r) {
}

void CPU::exec_MADD(r_type r) {
}

void CPU::exec_MSUB(r_type r) {
}

void CPU::exec_NMADD(r_type r) {
}

void CPU::exec_NMSUB(r_type r) {
}

void CPU::exec_OP_IMM(i_type i) {
  uint64_t rd_content;
  uint64_t rs1_content;
  GetReg(rs1_content, i.rs1);
  int64_t imm_signed;
  imm_signed = u64_to_i64(i.imm, 12);
  switch (i.func3) {
    case 0x0: // ADDI
      rd_content = rs1_content + imm_signed;
      break;
    case 0x1: // SLLI
      rd_content = rs1_content << (imm_signed & 0x1f);
      break;
    case 0x2: // SLTI
      rd_content = static_cast<int64_t>(rs1_content)
                 < imm_signed ? 1 : 0;
      break;
    case 0x3: // SLTIU
      rd_content = static_cast<uint64_t>(imm_signed)
                 > rs1_content ? 0 : 1;
      break;
    case 0x4: // XORI
      rd_content = rs1_content ^ imm_signed;
      break;
    case 0x6: // ORI
      rd_content = rs1_content | imm_signed;
      break;
    case 0x7: // ANDI
      rd_content = rs1_content & imm_signed;
      break;
    case 0x5: // SRLI | SRAI
      if (imm_signed & 0x400 == 0)
        rd_content = static_cast<uint64_t>(rs1_content)
                   >> (imm_signed & 0x1f);
      else
        rd_content = static_cast<int64_t>(rs1_content)
                   >> (imm_signed & 0x1f);
      break;
    default:
      throw "Exception: Unexpected func3";
  }
  SetReg(rd_content, i.rd);
}

void CPU::exec_JALR(i_type i) {
}
void CPU::exec_LOAD(i_type i) {
}

void CPU::exec_MISC_MEM(i_type i) {
}

void CPU::exec_SYSTEM(i_type i) {
}

void CPU::exec_OP_IMM_32(i_type i) {
}

void CPU::exec_LOAD_FP(i_type i) {
}

void CPU::exec_STORE(s_type s) {
}

void CPU::exec_BRANCH(sb_type sb) {
}

void CPU::exec_LUI(u_type u) {
  int32_t imm_signed32 = u.imm << 12;
  SetReg(imm_signed32, u.rd);
}

void CPU::exec_AUIPC(u_type u) {
  uint64_t curr_pc;
  int32_t imm_signed32 = u.imm << 12;
  GetPC(curr_pc);
  SetReg(imm_signed32 + curr_pc - 4, u.rd);
}

void CPU::exec_JAL(uj_type uj) {
  uint64_t curr_pc;
  int32_t imm_signed32 = uj.imm;
  GetPC(curr_pc);
  SetReg(imm_signed32 + curr_pc - 4, uj.rd);
}

