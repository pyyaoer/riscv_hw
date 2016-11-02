#include "read_elf.h"

int main(int argc, char** argv){
  if (argc != 2){
    return -1;
  }
  ReadELF read_elf(argv[1]);
  read_elf.AnalyzeELF();
  read_elf.PrintELFHeader();
  read_elf.PrintProgramHeader();
  read_elf.PrintSectionHeader();
  return 0;
}
