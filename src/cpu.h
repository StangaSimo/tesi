#include "util.h"

struct Registers
{
    int32_t r[32];
    int16_t im;
    bool c : 1;
    bool i : 1;
    int32_t pc;
};

struct Memory
{
    int32_t *data;
    int32_t size;
};

struct Instruction 
{
    int8_t type : 1;   /* 1 type A, 0 type B */
    int8_t opcode : 6;
    int8_t rd : 5;
    int8_t ra : 5;
    int8_t rb : 5;
    int32_t im : 16;
};

struct Memory *inizialize_memory(int32_t size, struct Memory *Data);
struct Registers *inizialize_registers(struct Registers *Reg);
void print_registers(struct Registers *Reg);
void run_instruction(int8_t* instr, struct Memory *Data, struct Registers *Reg, int8_t **instructions);
struct Instruction* parse_instruction(int8_t* instr, int8_t type, struct Instruction* res, int16_t *im);
void print_instr(struct Instruction *res, int8_t *instr);
int32_t signExtension(int16_t instr);
int32_t checkOverflow(int64_t n, bool* c);
void update_PC(struct Registers *Reg,int32_t n);
bool cmp_opcode(int8_t a,int8_t b);
bool cmp_reg(int8_t a,int8_t b);