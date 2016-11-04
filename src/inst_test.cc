#include "include/inst.h"

const char* g_OpType[] = {
           "LOAD", "LOAD_FP", "MISC_MEM", "OP_IMM", "AUIPC",
           "OP_IMM_32", "STORE", "STORE_FP", "AMO", "OP", "LUI",
           "OP_32", "MADD", "MSUB", "NMSUB", "NMADD", "OP_FP",
           "BRANCH", "JALR", "JAL", "SYSTEM", "ERROR" };
const char* g_InstFormat[] = {
           "R_TYPE", "I_TYPE", "S_TYPE", "SB_TYPE",
           "U_TYPE", "UJ_TYPE" };

int main(void){
  try {
    uint64_t inst_test = 0x13;
    Inst inst(inst_test);

    enum OpType type;
    inst.GetOpType(type);
    printf("OpType: %s\n", g_OpType[type]);

    union InstArgs args;
    enum InstFormat form;
    inst.GetArgs(args, form);
    printf("Format: %s\n", g_InstFormat[form]);
  } catch (const char* e){
    printf("Exception: %s\n", e);
  }
}
