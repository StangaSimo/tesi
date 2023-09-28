#include "../src/util.h"
#include "../src/cpu.h"

#define MEMORY_SIZE 1024
#define ADD_RESULT 159
#define SUB_RESULT -2147482620
#define BIT_OP_RESULT 200
#define BRANCH_RESULT 46
#define ABSOLUTE_BRANCH_RESULT 60
#define CMP_RESULT -146800670
#define DATA_RESULT 199
#define DEBUG 

bool test (char* file,int32_t n) 
{
    int32_t instructions_size = 0;
    int8_t **instructions = get_instructions_from_file(file, &instructions_size);

#ifdef DEBUG
    print_bit(instructions,instructions_size);
#endif

    struct Memory *data = malloc(sizeof(struct Memory));
    data = inizialize_memory(MEMORY_SIZE, data);

    struct Registers *reg = malloc(sizeof(struct Registers));
    reg = inizialize_registers(reg);

    while (reg->pc < instructions_size)
    {
        run_instruction(instructions[reg->pc], data, reg, instructions, false);
    }

#ifdef DEBUG
    print_registers(reg);
    printf ("r1: %d expected: %d\n",reg->r[1],n);
#endif

    if (reg->r[1] == n)
        return true;
    return false;
}

int main () 
{
    printf("***************************\n");

    if (test("test/add.text",ADD_RESULT)) 
        printf("ADD test OK\n");
    else 
        printf("ADD test FAIL\n");   

    printf("***************************\n");

    if (test("test/sub.text",SUB_RESULT)) 
        printf("SUB test OK\n");
    else 
        printf("SUB test FAIL\n");   

    printf("***************************\n");

    if (test("test/bitop.text",BIT_OP_RESULT)) 
        printf("BIT_OP test OK\n");
    else 
        printf("BIT_OP test FAIL\n");   

    printf("***************************\n");

    if (test("test/branch.text",BRANCH_RESULT)) 
        printf("BRANCH test OK\n");
    else 
        printf("BRANCH test FAIL\n");   

    printf("***************************\n");

    if (test("test/absolute_branch.text",ABSOLUTE_BRANCH_RESULT)) 
        printf("ABSOLUTE BRANCH test OK\n");
    else 
        printf("ABSOLUTE BRANCH test FAIL\n");   

    printf("***************************\n");

    if (test("test/cmp.text",CMP_RESULT)) 
        printf("CMP test OK\n");
    else 
        printf("CMP test FAIL\n");   

    printf("***************************\n");

    if (test("test/data.text",DATA_RESULT)) 
        printf("DATA test OK\n");
    else 
        printf("DATA test FAIL\n");   

    printf("***************************\n");
    return EXIT_SUCCESS;
}

