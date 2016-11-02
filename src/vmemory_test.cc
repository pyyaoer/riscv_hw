#include <stdio.h>
#include "include/vmemory.h"

int main(void){
  char input = 't';
  char output = 'T';
  VMemory vm;
  vm.PutByte(123LL, input);
  vm.GetByte(123LL, output);
  printf("input: %c\n", input);
  printf("output: %c\n", output);
  return 0;
}
