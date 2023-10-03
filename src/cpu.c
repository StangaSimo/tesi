#include "cpu.h"
#include "util.h"

struct Memory *inizialize_memory(int32_t size, struct Memory *data)
{
    data->size = size;
    data->data = calloc(data->size, sizeof(int32_t));
    memset(data->data, 0, sizeof(int32_t) * data->size);
    return data;
}

struct Registers *inizialize_registers(struct Registers *reg)
{
    for (int i = 0; i < 32; i++)
    {
        reg->r[i] = 0;
    }
    reg->c = false;
    reg->i = false;
    reg->pc = 0;
    reg->im = 0;
    return reg;
}

/* opcode: 6 bit  rd,ra,rb: 5 bit
   opcode: 6 bit  rd,ra: 5 bit  imm: 16 bit */
struct Instruction *parse_instruction(int8_t *instr, int8_t type, struct Instruction *res, int16_t *im)
{
    if (type) /* Type A */
    {
        res->type = type;
        res->rd = (instr[0] << 3) + ((instr[1] >> 5) & 0b00000111);
        res->ra = instr[1] & 0b00011111;
        res->rb = (instr[2] >> 3) & 0b00011111;
    }
    else /* Type B */
    {
        res->type = type;
        res->rd = ((instr[0] << 3) & 0b00011000) + ((instr[1] >> 5) & 0b00000111);
        res->ra = instr[1] & 0b00011111;
        int16_t n = instr[2];
        n = (n << 8) + (((int16_t)instr[3]) & 0b0000000011111111);

        if (*im) /* imm istruction before */
        {
            res->im = (*im << 16) + ((int32_t)n & 0b00000000000000001111111111111111);
            *im = 0;
        }
        else
            res->im = (int32_t)n;
    }
    return res;
}

void run_instruction(int8_t *instruction, struct Memory *data, struct Registers *reg, int8_t **instructions, bool delay)
{
    struct Instruction *instr = malloc(sizeof(struct Instruction));
    bool carry = 0; // carry
    instr->opcode = (instruction[0] >> 2) & 0b00111111;
    int8_t op_code = (instruction[0] >> 2) & 0b00111111; /* remove last 11 bitfield is not working for the == operand*/
    int32_t delayed_instruction, addr;
    int8_t branch_type, is_delayed, is_absolute, is_link;

    switch (op_code) {

        case 0x0 : /* ADD 000000 */
            instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
            reg->r[instr->rd] = add_Check_Overflow(reg->r[instr->ra], reg->r[instr->rb], &carry);
            update_PC(reg, 4, delay);
            break;

        case  0x4 : /* ADDK 000100 */
            instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
            reg->r[instr->rd] = add_Check_Overflow(reg->r[instr->ra], reg->r[instr->rb], &carry);
            reg->c = carry;
            update_PC(reg, 4, delay);
            break;
        case  0x2 : /* ADDC 000010 */
            instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
            reg->r[instr->rd] = add_Check_Overflow(reg->r[instr->ra], reg->r[instr->rb] + reg->c, &carry);
            update_PC(reg, 4, delay);
            break;
        case  0x6 : /* ADDCK 000110 */
            instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
            reg->r[instr->rd] = add_Check_Overflow(reg->r[instr->ra], reg->r[instr->rb] + reg->c, &carry);
            reg->c = carry;
            update_PC(reg, 4, delay);
            break;

        case  0x8 : /* ADDI 001000 */
            instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
            reg->r[instr->rd] = add_Check_Overflow(instr->im, reg->r[instr->ra], &carry);
            update_PC(reg, 4, delay);
            break;

        case  0xA : /* ADDIC 001010 */
            instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
            reg->r[instr->rd] = add_Check_Overflow(instr->im, reg->r[instr->ra] + reg->c, &carry);
            update_PC(reg, 4, delay);
            break;

        case  0xC : /* ADDIK 001100 */
            instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
            reg->r[instr->rd] = add_Check_Overflow(instr->im, reg->r[instr->ra], &carry);
            reg->c = carry;
            update_PC(reg, 4, delay);
            break;

        case  0xE : /* ADDICK 001110 */
            instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
            reg->r[instr->rd] = add_Check_Overflow(instr->im, reg->r[instr->ra] + reg->c, &carry);
            reg->c = carry;
            update_PC(reg, 4, delay);
            break;

        case  0x1 : /* RSUB 000001 */
            instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
            reg->r[instr->rd] = add_Check_Overflow(reg->r[instr->rb], add_Check_Overflow(~reg->r[instr->ra], 1, &carry), &carry);
            update_PC(reg, 4, delay);
            break;

        case  0x3 : /* RSUBC 000011 */
            instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
            reg->r[instr->rd] = add_Check_Overflow(reg->r[instr->rb], add_Check_Overflow(~reg->r[instr->ra], 1, &carry) + reg->c, &carry);
            update_PC(reg, 4, delay);
            break;

        case  0x5 : /* RSUBK CMP 000101 */

            /* last 8 bit 00000011  then cmpu */
            instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);

            if (instruction[3] == 0x0) {            
                instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
                reg->r[instr->rd] = add_Check_Overflow(reg->r[instr->rb], add_Check_Overflow(~reg->r[instr->ra], 1, &carry), &carry);
                reg->c = carry;

            } else {
                if (instruction[3] == 0x3) {
                    reg->r[instr->rd] = ((uint32_t)reg->r[instr->rb]) + (~((uint32_t)reg->r[instr->ra]) + 1);

                    if (((uint32_t)reg->r[instr->ra]) > ((uint32_t)reg->r[instr->rb]))
                        reg->r[instr->rd] = (reg->r[instr->rd] & 0x7FFFFFFF) + 0x80000000; /* (rD)(MSB) ← (rA) > (rB) */
                } else {
                    reg->r[instr->rd] =reg->r[instr->rb] + (~reg->r[instr->ra] + 1);

                    if (reg->r[instr->ra] > reg->r[instr->rb])
                        reg->r[instr->rd] = (reg->r[instr->rd] & 0x7FFFFFFF) + 0x80000000; 
                }
            }
            update_PC(reg, 4, delay);
            break;

        case  0x7 : /* RSUBCK 000111 */
            instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
            reg->r[instr->rd] = add_Check_Overflow(reg->r[instr->rb], add_Check_Overflow(~reg->r[instr->ra], 1, &carry) + reg->c, &carry);
            reg->c = carry;
            update_PC(reg, 4, delay);
            break;

        case  0x9 : /* RSUBI 001001 */
            instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
            reg->r[instr->rd] = add_Check_Overflow(instr->im, add_Check_Overflow(~reg->r[instr->ra], 1, &carry), &carry);
            update_PC(reg, 4, delay);
            break;

        case  0xB : /* RSUBIC 001011 */
            instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
            reg->r[instr->rd] = add_Check_Overflow(instr->im, add_Check_Overflow(~reg->r[instr->ra], 1, &carry) + reg->c, &carry);
            update_PC(reg, 4, delay);
            break;

        case  0xD : /* RSUBIK 001101 */
            instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
            reg->r[instr->rd] = add_Check_Overflow(instr->im, add_Check_Overflow(~reg->r[instr->ra], 1, &carry), &carry);
            reg->c = carry;
            update_PC(reg, 4, delay);
            break;

        case  0xF : /* RSUBIKC 001111 */
            instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
            reg->r[instr->rd] = add_Check_Overflow(instr->im, add_Check_Overflow(~reg->r[instr->ra], 1, &carry) + reg->c, &carry);
            reg->c = carry;
            update_PC(reg, 4, delay);
            break;

        case  0x24 : /* SRA 100100 */
            // TODO: carry?
            instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
            reg->r[instr->rd] = reg->r[instr->ra] >> 1;
            reg->c = carry;
            update_PC(reg, 4, delay);
            break;

        case  0x20 : /* OR 100000 */
            instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
            reg->r[instr->rd] = reg->r[instr->ra] | reg->r[instr->rb];
            update_PC(reg, 4, delay);
            break;

        case  0x21 : /* AND 100001 */
            instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
            reg->r[instr->rd] = reg->r[instr->ra] & reg->r[instr->rb];
            update_PC(reg, 4, delay);
            break;

        case  0x22 : /* XOR 100010 */
            instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
            reg->r[instr->rd] = reg->r[instr->ra] ^ reg->r[instr->rb];
            update_PC(reg, 4, delay);
            break;

        case  0x23 : /* ANDN 100011 */
            instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
            reg->r[instr->rd] = reg->r[instr->ra] & (~reg->r[instr->rb]);
            update_PC(reg, 4, delay);
            break;

        case  0x28 : /* ORI 101000 */
            instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
            reg->r[instr->rd] = reg->r[instr->ra] | instr->im;
            update_PC(reg, 4, delay);
            break;

        case  0x29 : /* ANDI 101001 */
            instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
            reg->r[instr->rd] = reg->r[instr->ra] & instr->im;
            update_PC(reg, 4, delay);
            break;

        case  0x2A : /* XORI 101010 */
            instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
            reg->r[instr->rd] = reg->r[instr->ra] ^ instr->im;
            update_PC(reg, 4, delay);
            break;

        case  0x2B : /* ANDNI 101011 */
            instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
            reg->r[instr->rd] = reg->r[instr->ra] & (~instr->im);
            update_PC(reg, 4, delay);
            break;

        case  0x27: /****************************** BEQ BGE BGT BLE BLT BNE 100111 */
            instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
            delayed_instruction = reg->pc + 1; /* next istruction for delayed branch*/
            branch_type = conv_reg(instr->rd) & 0b00001111;
            is_delayed = conv_reg(instr->rd) & 0b00010000;

            if (branch_type == 0x0 && reg->r[instr->ra] == 0x0) /* BEQ D0000 */
                update_PC(reg, reg->r[instr->rb], delay);
            else if (branch_type == 0x5 && reg->r[instr->ra] >= 0x0) /* BGE D0101 */
                update_PC(reg, reg->r[instr->rb], delay);
            else if (branch_type == 0x4 && reg->r[instr->ra] > 0x0) /* BGT D0100 */
                update_PC(reg, reg->r[instr->rb], delay);
            else if (branch_type == 0x3 && reg->r[instr->ra] <= 0x0) /* BLE D0011 */
                update_PC(reg, reg->r[instr->rb], delay);
            else if (branch_type == 0x2 && reg->r[instr->ra] < 0x0) /* BLT D0010 */
                update_PC(reg, reg->r[instr->rb], delay);
            else if (branch_type == 0x1 && reg->r[instr->ra] != 0x0) /* BNE D0001 */
                update_PC(reg, reg->r[instr->rb], delay);
            else
                update_PC(reg, 4, delay);

            if (is_delayed == 0x10) /* delayed slot */
                run_instruction(instructions[delayed_instruction], data, reg, instructions, true);
            break;

        case  0x2F : /******************************  BEQI BGEI BGTI BLEI BLTI BNEI 101111 */
            instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
            delayed_instruction = reg->pc + 1; /* next istruction for delayed branch*/
            branch_type = conv_reg(instr->rd) & 0b00001111;
            is_delayed = conv_reg(instr->rd) & 0b00010000;

            if (branch_type == 0x0 && reg->r[instr->ra] == 0x0) /* BEQI D0000 */
                update_PC(reg, instr->im, delay);
            else if (branch_type == 0x5 && reg->r[instr->ra] >= 0x0) /* BGEI D0101 */
                update_PC(reg, instr->im, delay);
            else if (branch_type == 0x4 && reg->r[instr->ra] > 0x0) /* BGTI D0100 */
                update_PC(reg, instr->im, delay);
            else if (branch_type == 0x3 && reg->r[instr->ra] <= 0x0) /* BLEI D0011 */
                update_PC(reg, instr->im, delay);
            else if (branch_type == 0x2 && reg->r[instr->ra] < 0x0) /* BLTI D0010 */
                update_PC(reg, instr->im, delay);
            else if (branch_type == 0x1 && reg->r[instr->ra] != 0x0) /* BNEI D0001 */
                update_PC(reg, instr->im, delay);
            else
                update_PC(reg, 4, delay);

            if (is_delayed == 0x10) /* delayed slot */
                run_instruction(instructions[delayed_instruction], data, reg, instructions, true);
            break;

        case  0x26 : /* BR BRD BRA BRLD BRAD BRALD 101110 */
            instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
            delayed_instruction = reg->pc + 1; /* next istruction for delayed branch*/

            /* DAL00 */
            is_absolute = conv_reg(instr->ra) & 0b00001000; /* A for branch with absolute PC = rb */
            is_link = conv_reg(instr->ra) & 0b00000100;     /* L for branch and link rd = PC */
            is_delayed = conv_reg(instr->ra) & 0b00010000;

            if (is_link == 0x4)
                reg->r[conv_reg(instr->rd)] = reg->pc*4;

            if (is_absolute == 0x8)
                reg->pc = (reg->r[conv_reg(instr->rb)])/4;
            else
                update_PC(reg, reg->r[instr->rb], delay);

            if (is_delayed == 0x10) /* delayed slot */
                run_instruction(instructions[delayed_instruction], data, reg, instructions, true);
            break;

        case  0x2E : /* BRI BRAI BRID BRAID BRLID BRAILD 101110 */
            instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
            delayed_instruction = reg->pc + 1; /* next istruction for delayed branch*/

            /* DAL00 */
            is_absolute = conv_reg(instr->ra) & 0b00001000; /* A for branch with absolute PC = rb */
            is_link = conv_reg(instr->ra) & 0b00000100;     /* L for branch and link rd = PC */
            is_delayed = conv_reg(instr->ra) & 0b00010000;

            if (is_link == 0x4)
                reg->r[conv_reg(instr->rd)] = reg->pc*4;

            if (is_absolute == 0x8)
                reg->pc = instr->im/4;
            else
                update_PC(reg, instr->im, delay);

            if (is_delayed == 0x10) /* delayed slot */
                run_instruction(instructions[delayed_instruction], data, reg, instructions, true);
            break;

        case  0x2D : /* RTSD 101101 */
            instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
            delayed_instruction = reg->pc + 1; /* next istruction for delayed branch*/
            if (conv_reg(instr->rd) == 0x10) {
                reg->pc = (reg->r[instr->ra] + instr->im)/4;
                run_instruction(instructions[delayed_instruction], data, reg, instructions, true);
            }
            break;

        case  0x32 : /* LW 110010 */
            instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
            uint32_t addr = (uint32_t)(reg->r[instr->ra] + reg->r[instr->rb]);
            reg->r[instr->rd] = data->data[addr];
            update_PC(reg, 4, delay);
            break;

        case  0x36 : /* SW 110110 */
            instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
            addr = (uint32_t)(reg->r[instr->ra] + reg->r[instr->rb]);
            data->data[addr] = reg->r[instr->rd];
            update_PC(reg, 4, delay);
            break;

        case  0x3A : /* LWI 111010 */
            instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
            addr = (uint32_t)(reg->r[instr->ra] + instr->im);
            reg->r[instr->rd] = data->data[addr] ;
            update_PC(reg, 4, delay);
            break;

        case  0x3E : /* SWI 111110 */
            instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
            addr = (uint32_t)(reg->r[instr->ra] + instr->im);
            data->data[addr] = reg->r[instr->rd];
            update_PC(reg, 4, delay);
            break;

        case  0x2C : /* IMM 101100 */
            instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
            reg->im = instr->im;
            update_PC(reg, 4, delay);
            break;

        default : 
            printf("unknown istruction\n");
            break;
    }
}

void print_instr(struct Instruction *res, int8_t *instr)
{
    printf("\ninstr: ");
    for (int i = 0; i < 4; i++)
        for (int z = 7; 0 <= z; z--)
            printf("%c", (instr[i] & (1 << z)) ? '1' : '0');
    printf("\nopcode: ");
    for (int z = 5; 0 <= z; z--)
        printf("%c", (res->opcode & (1 << z)) ? '1' : '0');
    printf("\nrd: ");
    for (int z = 4; 0 <= z; z--)
        printf("%c", (res->rd & (1 << z)) ? '1' : '0');
    printf("\nra: ");
    for (int z = 4; 0 <= z; z--)
        printf("%c", (res->ra & (1 << z)) ? '1' : '0');
    if (res->type)
    {
        printf("\nrb: ");
        for (int z = 4; 0 <= z; z--)
            printf("%c", (res->rb & (1 << z)) ? '1' : '0');
    }
    else
    {
        printf("\nim: ");
        for (int z = 15; 0 <= z; z--)
            printf("%c", (res->im & (1 << z)) ? '1' : '0');
        printf("  intero: %d ", (res->im)); /* complemento 2 */
    }
    printf("\n");
}

void print_registers(struct Registers *reg)
{
    printf("\n============================= Register Status =============================\n");
    printf("\n                           c: %d     i: %d    pc: %d\n\n", reg->c, reg->i, reg->pc);
    for (int i = 0; i < 32; i++)
    {
        printf("r%d: ", i);
        int32_t a = reg->r[i];
        if (i < 10)
            printf(" ");
        for (int z = 31; 0 <= z; z--)
        {
            printf("%c", (a & (1 << z)) ? '1' : '0');
        }
        if (i % 2 == 1)
            printf("\n");
        else
            printf("  ");
    }
    printf("\n===========================================================================\n");
}

// TODO: carry even in underflow?
int32_t add_Check_Overflow(int32_t a, int32_t b, bool *c)
{
    int64_t res = (int64_t)a + (int64_t)b; /* only last 32 bit*/
    if (res > INT32_MAX)
    {
        *c = true;
        res = INT32_MAX;
    }
    else if (res < INT32_MIN) /* underflow */
    {

        *c = true;
        res = INT32_MIN;
    }
    else
    {
        *c = false;
    }

    return (int32_t)res;
}

void update_PC(struct Registers *reg, int32_t n, bool delay)
{
    if (!delay) /* Note: if the instruction is in a delay slot, it shouldn't modify the pc reg*/
        reg->pc = reg->pc + n / 4;
}

int8_t conv_reg(int8_t n)
{
    return n & 0b00011111;
}
