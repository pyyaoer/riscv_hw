#include "include/inst.h"

int main(void){
  try {
    Inst inst(0x0);
  } catch (const char* e){
    printf("Exception: %s\n", e);
  }
}
