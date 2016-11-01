#ifndef RISCV_VM_VMEMORY_H_
#define RISCV_VM_VMEMORY_H_

#include <fcntl.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <vector>
using std::vector;

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&); \
  void operator=(const TypeName&)

class VPage;

class VMemory {
 public:
  VMemory(){}
  ~VMemory(){}
  void AccessByte(long long addr, char& content);

 private:
  vector<VPage> pages_;
  DISALLOW_COPY_AND_ASSIGN(VMemory);
};

class VPage {
 public:
  VPage() {VPage(4096);}
  explicit VPage(unsigned _s): size_(_s) {
    if (size_ <= 0 || (size_ & (size_-1)) != 0){
      throw "Unexpected page size.";
    }
    data_ = new char[size_];
    memset(data_, 0, size_);
  }
  ~VPage() {delete[] data_;}
  void AccessByte(unsigned vpo);

 private:
  unsigned size_;
  char* data_;
  DISALLOW_COPY_AND_ASSIGN(VPage);
};

#endif //RISCV_VM_VMEMORY_H_
