#include "include/cpu.h"

int main(void) {
  try {
    uint64_t init_pc = 0x1000;
    union {
      char c_inst[4] = {0};
      uint32_t i_inst;
    } inst;
    inst.i_inst = 0x4013;
    CPU cpu(init_pc);
    VMemory vm;
    cpu.SetMem(&vm);
    vm.PutNByte(init_pc, inst.c_inst, 4);
    while(1) {
      cpu.Fetch();
      cpu.Decode();
      cpu.Execute();
    }
  } catch (const char* e) {
    printf("%s\n", e);
  }
  return 0;
}
