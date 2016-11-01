#include <stdio.h>
#include "vmemory.h"

int main(void){
  char content;
  VMemory vm;
  vm.AccessByte(123LL, content);
  printf("%d\n", static_cast<int>(content));
  return 0;
}
