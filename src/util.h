#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define TYPE_A 1
#define TYPE_B 0

void print_bit_hex(int8_t **instr, int32_t size); 
void print_bit(int8_t **instr, int32_t size);
void print_hex(int8_t **instr, int32_t size);
int8_t **get_instructions_from_file(char *file_name, int32_t *size);