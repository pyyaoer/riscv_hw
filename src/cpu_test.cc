#include "include/cpu.h"

int main(void) {
  try {
    CPU cpu(0x400);
    VMemory vm;
    cpu.SetMem(&vm);
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
