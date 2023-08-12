#include "util.h"

void print_bit_hex(int8_t **instr, int32_t size) 
{
    printf("\n");
    int c = 0;
    for (int i = 0; i < size; i++)
    {
        if (i < 10)
            printf(" ");

        printf("%d:", c++);

        for (int j=0;j<4;j++)
            printf(" %02x ", instr[i][j]);

        for (int j=0;j<4;j++)
            for (int z = 7; 0 <= z; z--)
                printf("%c", (instr[i][j] & (1 << z)) ? '1' : '0');

        printf("   ");

        if (i%2 == 1)
            printf("\n");
    }
    printf("\n");
}

void print_bit(int8_t **instr, int32_t size)
{
    printf("\n");
    int c = 0;
    for (int i = 0; i < size; i++)
    {
        if (i<10)
            printf(" ");

        printf("%d: ", c++);
        for (int j=0;j<4;j++)
            for (int z = 7; 0 <= z; z--)
                printf("%c", (instr[i][j] & (1 << z)) ? '1' : '0');


        for (int j=0;j<4;j++)
            printf(" valore intero: %d",instr[i][j]);
        printf("\n");
    }
    printf("\n");
}

void print_hex(int8_t **instr, int32_t size)
{
    printf("\n");
    int c = 0;
    for (int i = 0; i < size; i++)
    {
        if (i < 10)
            printf(" ");

        printf("%d:", c++);

        for (int j=0;j<4;j++)
            printf(" %02x ", instr[i][j]);

        printf("\n");
    }
    printf("\n");
}

int8_t reverseBits(int8_t num)
{
    int8_t NO_OF_BITS = 32;
    int8_t reverse_num = 0;
    int i;
    for (i = 0; i < NO_OF_BITS; i++) {
        if ((num & (1 << i)))
            reverse_num |= 1 << ((NO_OF_BITS - 1) - i);
    }
    return reverse_num;
}

int8_t **get_instructions_from_file(char *file_name, int32_t *size)
{
    FILE *file = fopen(file_name, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Error: cannot open the file '%s'.\n", file_name);
        exit(1);
    }

    int8_t **instr = malloc(sizeof(int8_t*));

    fseek(file, 0, SEEK_SET);
    while (true)
    {
        int8_t *buffer = malloc(sizeof(int8_t)*4);
        int num_bytes = fread(buffer, sizeof(int8_t), 4, file);
        if (num_bytes > 0)
        {
            *size = *size + 1;
            instr = realloc(instr, (*size) * sizeof(int8_t*));
            instr[*size - 1] = buffer;
        }
        else
        {
            break;
        }
    }
    fclose(file);
    return instr;
}
