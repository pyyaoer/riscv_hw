#define LOG_FOR_TEST

#include "read_elf.h"

int main(void){
  ReadELF read_elf("riscv_hello");
  read_elf.AnalyzeELF();
  read_elf.PrintELFHeader();
  read_elf.PrintProgramHeader();
  read_elf.PrintSectionHeader();
  return 0;
}
