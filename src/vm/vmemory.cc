#include "vmemory.h"

bool IsValidPageSize(uint32_t psize){
    return psize >= 1024 && ((psize & (psize - 1)) == 0);
}

VMemory::VMemory(uint32_t _p): psize_(_p) {
  if (!IsValidPageSize(psize_)){
    throw "Unexpected page size.";
  }
}

VMemory::~VMemory() {
  for (auto page_iter = pages_.begin(); page_iter != pages_.end();
         ++ page_iter) {
    VPage* vp = page_iter->second;
    delete vp;
  }
}

void VMemory::AccessByte(uint64_t addr, char& content) {
  uint64_t vpn = addr / psize_;
  uint32_t vpo = static_cast<uint32_t>(addr & (psize_-1));
  auto page_it = pages_.find(vpn);
  if (page_it == pages_.end()) {
    // Malloc a new page here
    VPage* new_page = new VPage(psize_);
    pages_.insert(pair<uint64_t, VPage*>(vpn, new_page));
    page_it = pages_.find(vpn);
  }
  page_it->second->AccessByte(vpo, content);
}


VPage::VPage(uint32_t _s): size_(_s) {
  if (!IsValidPageSize(size_)){
    throw "Unexpected page size.";
  }
  data_ = new char[size_];
  memset(data_, 0, size_);
}

void VPage::AccessByte(unsigned vpo, char& content){
  if (vpo > size_) {
    throw "Unexpected page offset.";
  }
  content = data_[vpo];
}

