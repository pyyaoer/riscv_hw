#ifndef RISCV_ELF_READ_ELF_H_
#define RISCV_ELF_READ_ELF_H_

#include <elf.h>
#include <fcntl.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
using std::string;

#define LOG_FOR_TEST
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
  // Show elements in the header for DEBUG
  void PrintELFHeader();

  string file_name() const { return file_name_; }
  void set_file_name(const string &name) {
    file_name_.assign(name);
  }

 private:

  // Fill the ELF Header structure.
  // [in] fd: ELF file descriptor
  // return value:
  //   True for success
  bool VerifyELFFormat(int fd);
  void PrintELF32Header();
  void PrintELF64Header();

  char elf_ident_[EI_NIDENT];
  string file_name_;
  Elf32_Ehdr elf32_hdr_;
  Elf64_Ehdr elf64_hdr_;
  DISALLOW_COPY_AND_ASSIGN(ReadELF);
};

#endif //RISCV_ELF_READ_ELF_H_
