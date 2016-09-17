#define LOG_FOR_TEST

#include "read_elf.h"

int main(void){
  ReadELF read_elf("riscv_hello");
  read_elf.AnalyzeELF();
  return 0;
}
