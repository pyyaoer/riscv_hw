#include <string.h>
#include "include/cpu.h"


inline static void IsValidRegIndex(uint32_t n) {
  if (n >= 32) {
    throw "Exception: Unexpected Register Access";
  }
}

CPU::CPU(uint64_t pc) {
  pc_ = 0;
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
}

void CPU::Decode() {
}

void CPU::Execute() {
  throw "Program terminated successfully";
}
