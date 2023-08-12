#include "util.h"
#include "cpu.h"

#define MEMORY_SIZE 1024

int main(int argc, char *argv[])
{
    int32_t instructions_size = 0;
    int8_t **instructions = get_instructions_from_file(argv[1], &instructions_size);
    print_hex(instructions, instructions_size);
    print_bit(instructions, instructions_size);
    //print_bit_hex(instructions,instructions_size);

    struct Memory *data = malloc(sizeof(struct Memory));
    data = inizialize_memory(MEMORY_SIZE, data);

    struct Registers *reg = malloc(sizeof(struct Registers));
    reg = inizialize_registers(reg);

    while (reg->pc/4 < instructions_size) 
        run_instruction(instructions[reg->pc/4], data, reg, instructions);

    print_registers(reg);

    printf("\nr1: %d \n",reg->r[1]);


    //printf("=====================\nstart \n=====================\n");
    //printf("=====================\nend   \n=====================\n");
}