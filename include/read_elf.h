#ifndef RISCV_READ_ELF_H_
#define RISCV_READ_ELF_H_

#include <elf.h>
#include <fcntl.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <vector>
#include <unistd.h>
using std::string;
using std::vector;

// #define LOG_FOR_TEST
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&); \
  void operator=(const TypeName&)

const char kELFMagic[] = "\x7f\x45\x4c\x46";

class ReadELF {
 public:
  ReadELF(): file_name_("") {
    memset(elf_ident_, 0, sizeof(elf_ident_));
  }
  explicit ReadELF(const char *name) : file_name_(name) {
    memset(elf_ident_, 0, sizeof(elf_ident_));
  }
  explicit ReadELF(const string &name) : file_name_(name) {
    memset(elf_ident_, 0, sizeof(elf_ident_));
  }
  ~ReadELF() {}

  bool AnalyzeELF();
  // Fill the ELF Header structure.
  // [in] fd: ELF file descriptor
  // return value:
  //   True for success
  bool AnalyzeELFHeader(int fd);
  // Fill the Program Header vector.
  // [in] fd: ELF file descriptor
  // return value:
  //   True for success
  bool AnalyzeProgramHeader(int fd);
  // Fill the Sector Header vector.
  // [in] fd: ELF file descriptor
  // return value:
  //   True for success
  bool AnalyzeSectionHeader(int fd);
  // Show elements in the ELF header.
  bool AnalyzeSectionName(int fd);
  // Show elements in the ELF header.
  void PrintELFHeader();
  // Show elements in the program headers.
  void PrintProgramHeader();
  // Show elements in the section headers.
  void PrintSectionHeader();

  string file_name() const { return file_name_; }
  void set_file_name(const string &name) {
    file_name_.assign(name);
  }

 private:
  // 32-bit & 64-bit for AnalyzeProgramHeader(int)
  bool AnalyzeProgramHeader32(int fd);
  bool AnalyzeProgramHeader64(int fd);
  void PrintProgramHeader32(Elf32_Phdr *hdr);
  void PrintProgramHeader64(Elf64_Phdr *hdr);
  // 32-bit & 64-bit for AnalyzeSectionHeader(int)
  bool AnalyzeSectionHeader32(int fd);
  bool AnalyzeSectionHeader64(int fd);
  void PrintSectionHeader32(Elf32_Shdr *hdr);
  void PrintSectionHeader64(Elf64_Shdr *hdr);
  // 32-bit & 64-bit for AnalyzeSectionName(int)
  bool AnalyzeSectionName32(int fd);
  bool AnalyzeSectionName64(int fd);
  // 32-bit & 64-bit for PrintELFHeader()
  void PrintELF32Header();
  void PrintELF64Header();

  char elf_ident_[EI_NIDENT];
  string file_name_;
  Elf32_Ehdr elf32_hdr_;
  Elf64_Ehdr elf64_hdr_;
  vector<Elf32_Phdr> elf32_program_hdr_;
  vector<Elf64_Phdr> elf64_program_hdr_;
  vector<Elf32_Shdr> elf32_section_hdr_;
  vector<Elf64_Shdr> elf64_section_hdr_;
  vector<string> elf32_section_name_;
  vector<string> elf64_section_name_;
  DISALLOW_COPY_AND_ASSIGN(ReadELF);
};

#endif //RISCV_READ_ELF_H_
