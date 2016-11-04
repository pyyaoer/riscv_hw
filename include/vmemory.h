#ifndef RISCV_VMEMORY_H_
#define RISCV_VMEMORY_H_

#include <fcntl.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <map>
using std::map;
using std::pair;

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&); \
  void operator=(const TypeName&)

class VPage;

class VMemory {
 public:
  VMemory(): psize_(4096) {}
  explicit VMemory(uint32_t _p);
  ~VMemory();
  void GetByte(uint64_t addr, char& content);
  void PutByte(uint64_t addr, char content);

 private:
  VPage* TouchPage(uint64_t vpn);

  uint32_t psize_;
  map<uint64_t, VPage*> pages_;
  DISALLOW_COPY_AND_ASSIGN(VMemory);
};

class VPage {
 public:
  VPage() {VPage(4096);}
  explicit VPage(uint32_t _s);
  ~VPage() {delete[] data_;}
  void GetByte(uint32_t vpo, char& content);
  void PutByte(uint32_t vpo, char content);

 private:
  uint32_t size_;
  char* data_;
  DISALLOW_COPY_AND_ASSIGN(VPage);
};

#endif //RISCV_VMEMORY_H_
