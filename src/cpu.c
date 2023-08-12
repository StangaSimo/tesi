#include "cpu.h"
#include "util.h"

struct Memory *inizialize_memory(int32_t size, struct Memory *data)
{
    data->size = size;
    data->data = calloc(data->size, sizeof(int32_t));
    memset(data->data, 0, sizeof(int32_t) *data->size);
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
        res->rd = (instr[0] << 6) + (instr[1] >> 5);
        res->ra = (instr[1] << 3) >> 3;
        res->rb = instr[2] >> 3;
    }
    else /* Type B */
    {
        res->type = type;
        res->rd = (instr[0] << 6) + (instr[1] >> 5);
        res->ra = (instr[1] << 3) >> 3;
        int16_t n = instr[2];
        n = n << 8;

        if (*im) /* imm istruction before */
        {
            res->im = (((int32_t)*im) << 16) +(n + ((int16_t)instr[3] & 0b0000000011111111));
            *im = 0;
        } else 
            res->im = (int32_t)(n + ((int16_t)instr[3] & 0b0000000011111111));
    }
    return res;
}

void run_instruction(int8_t *instruction, struct Memory *data, struct Registers *reg, int8_t **instructions)
{
    struct Instruction *instr = malloc(sizeof(struct Instruction));
    bool carry=0; //carry
    instr->opcode = instruction[0] >> 2;
    int8_t op_code = (instruction[0] >> 2) & 0b00111111; /* remove last 11 bitfield is not working for the == operand*/

    if (op_code == 0x0)           /* ADD 000000 */ 
    { 
        instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
        reg->r[instr->rd] = checkOverflow((int64_t)(reg->r[instr->ra] + reg->r[instr->rb]),&carry);
        update_PC(reg,4);

    } else if (op_code == 0x4)    /* ADDK 000100 */
    {
        instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
        reg->r[instr->rd] = checkOverflow((int64_t)(reg->r[instr->ra] + reg->r[instr->rb]),&carry);
        reg->c = carry;
        update_PC(reg,4);

    } else if (op_code == 0x2)    /* ADDC 000010 */
    {
        instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
        reg->r[instr->rd] = checkOverflow((int64_t)(reg->r[instr->ra] + reg->r[instr->rb] + reg->c),&carry);
        update_PC(reg,4);

    } else if (op_code == 0x6)    /* ADDCK 000110 */
    {
        instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
        reg->r[instr->rd] = checkOverflow((int64_t)(reg->r[instr->ra] + reg->r[instr->rb] + reg->c),&carry);
        reg->c = carry;
        update_PC(reg,4);

    } else if (op_code == 0x8)   /* ADDI 001000 */
    {
        instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
        reg->r[instr->rd] = checkOverflow((int64_t)(instr->im + reg->r[instr->ra]),&carry);
        update_PC(reg,4);

    } else if (op_code == 0xA)   /* ADDIC 001010 */
    {
        instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
        reg->r[instr->rd] = checkOverflow((int64_t)(instr->im + reg->r[instr->ra] + reg->c),&carry);
        update_PC(reg,4);

    } else if (op_code == 0xC)   /* ADDIK 001100 */
    {
        instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
        reg->r[instr->rd] = checkOverflow((int64_t)(instr->im + reg->r[instr->ra]),&carry);
        reg->c = carry;
        update_PC(reg,4);

    } else if (op_code == 0xE)   /* ADDICK 001110 */
    {
        instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
        reg->r[instr->rd] = checkOverflow((int64_t)(instr->im + reg->r[instr->ra] + reg->c),&carry);
        reg->c = carry;
        update_PC(reg,4);

    } else if (op_code == 0x1)    /* RSUB 000001 */
    {
        instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
        reg->r[instr->rd] = checkOverflow((int64_t)(reg->r[instr->rb] + (~reg->r[instr->ra] + 1)),&carry);
        update_PC(reg,4);

    } else if (op_code == 0x3)   /* RSUBC 000011 */
    {
        instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
        reg->r[instr->rd] = checkOverflow((int64_t)(reg->r[instr->rb] + (~reg->r[instr->ra] + 1) + reg->c),&carry);
        update_PC(reg,4);

    } else if (op_code == 0x5)   /* RSUBK 000101 */
    {
        instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
        reg->r[instr->rd] = checkOverflow((int64_t)(reg->r[instr->rb] + (~reg->r[instr->ra] + 1)),&carry);
        reg->c = carry;
        update_PC(reg,4);

    } else if (op_code == 0x7)   /* RSUBCK 000111 */
    {
        instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
        reg->r[instr->rd] = checkOverflow((int64_t)(reg->r[instr->rb] + (~reg->r[instr->ra] + 1) + reg->c),&carry);
        reg->c = carry;
        update_PC(reg,4);

    } else if (op_code == 0x9)   /* RSUBI 001001 */
    {
        instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
        reg->r[instr->rd] = checkOverflow((int64_t)(instr->im + (~reg->r[instr->ra] + 1)),&carry);
        update_PC(reg,4);

    } else if (op_code == 0xB)   /* RSUBIC 001011 */
    {
        instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
        reg->r[instr->rd] = checkOverflow((int64_t)(instr->im + (~reg->r[instr->ra] + 1) + reg->c),&carry);
        update_PC(reg,4);

    } else if (op_code == 0xD)   /* RSUBIK 001101 */
    {
        instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
        reg->r[instr->rd] = checkOverflow((int64_t)(instr->im + (~reg->r[instr->ra] + 1)),&carry);
        reg->c = carry;
        update_PC(reg,4);

    } else if (op_code == 0xF)   /* RSUBIKC 001111 */
    {
        instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
        reg->r[instr->rd] = checkOverflow((int64_t)(instr->im + (~reg->r[instr->ra] + 1) + reg->c ),&carry);
        reg->c = carry;
        update_PC(reg,4);

    } else if (op_code == 0x24)  /* SRA 100100 */
    {
        //TODO: carry?
        instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
        reg->r[instr->rd] = reg->r[instr->ra] >> 1;
        reg->c = carry;

    } else if (op_code == 0x20)  /* OR 100000 */
    {
        instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
        reg->r[instr->rd] = reg->r[instr->ra] | reg->r[instr->rb];
        update_PC(reg,4);

    } else if (op_code == 0x21)   /* AND 100001 */
    {
        instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
        reg->r[instr->rd] = reg->r[instr->ra] & reg->r[instr->rb];
        update_PC(reg,4);

    } else if (op_code == 0x22)   /* XOR 100010 */
    {
        instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
        reg->r[instr->rd] = reg->r[instr->ra] ^ reg->r[instr->rb];
        update_PC(reg,4);

    } else if (op_code == 0x23)   /* ANDN 100011 */
    {
        instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
        reg->r[instr->rd] = reg->r[instr->ra] & (~reg->r[instr->rb]);
        update_PC(reg,4);

    } else if (op_code == 0x28)   /* ORI 101000 */
    {
        instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
        reg->r[instr->rd] = reg->r[instr->ra] | instr->im;
        update_PC(reg,4);

    } else if (op_code == 0x29)   /* ANDI 101001 */
    {
        instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
        reg->r[instr->rd] = reg->r[instr->ra] & instr->im;
        update_PC(reg,4);

    } else if (op_code == 0x2A)   /* XORI 101010 */
    {
        instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
        reg->r[instr->rd] = reg->r[instr->ra] ^ instr->im;
        update_PC(reg,4);

    } else if (op_code == 0xC)   /* ANDNI 001100 */
    {
        instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
        reg->r[instr->rd] = reg->r[instr->ra]  & (~instr->im);
        update_PC(reg,4);


    } else if (op_code == 0x2E)   /****************************** B 100111 */
    {
        instr = parse_instruction(instruction, TYPE_A, instr, &reg->im);
        int32_t delay = reg->pc + 1;
        if (instr->rd == 0x10 || instr->rd == 0x0) /* BEQ */
        {
            if (cmp_reg(reg->r[instr->ra],0x0)) //TODO: ha senso andare in questa direzione? 
                update_PC(reg, reg->r[instr->rb]);
            else
                update_PC(reg, 4);
            if (instr->rd == 0x10) /* BEQD rd = 10000*/
                run_instruction(instructions[delay], data, reg, instructions);
        }
        else if (instr->rd == 0x15 || 0x5)        /* BGE */
        {
            if (reg->r[instr->ra] >=  0x0)
                update_PC(reg, reg->r[instr->rb]);
            else
                update_PC(reg, 4);
            if (instr->rd == 0x15) /* BEGD */
                run_instruction(instructions[delay], data, reg, instructions);
        }
    }
    else if (op_code == 0x2F)    /******************************  BI 101111 */
    {
        instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
        int32_t delay = reg->pc + 1;
        if (instr->rd == 0x10 || instr->rd == 0x0) /* BEQI */
        {
            printf("ci sono %d r1 %d\n",reg->r[instr->ra],reg->r[1]);
            if (reg->r[instr->ra] == 0x0)
                update_PC(reg, instr->im);
            else
                update_PC(reg, 4);
            if (instr->rd == 0x10) /* BEQD */
                run_instruction(instructions[delay], data, reg, instructions);
        }
        else if (instr->rd == 0x15 || 0x5) /* BGEI */
        {
            if (reg->r[instr->ra] >=  0x0)
                update_PC(reg, reg->r[instr->rb]);
            else
                update_PC(reg, 4);
            if (instr->rd == 0x15) /* BGEID */
                run_instruction(instructions[delay], data, reg, instructions);
        }
    }
    else if (op_code == 0x2E) /* BRI 101110 */
    {

    }
    else if (op_code == 0xC) /* RTSD 101101 */
    {

    }
    else if (op_code == 0xC) /* RTID 101101 */
    {
    //} else if (op_code   == 0xC)  /* BEQI 101111 */
    //{
    //} else if (op_code   == 0xC)  /* BNEI 101111 */
    //{
    //} else if (op_code   == 0xC)  /* BLTI 101111 */
    //{
    //} else if (op_code   == 0xC)  /* BLEI 101111 */
    //{
    //} else if (op_code   == 0xC)  /* BGTI 001100 */
    //{
    //} else if (op_code   == 0xC)  /* BGEI 001100 */
    //{
    } else if (op_code == 0xC)  /* LW 110010 */
    {
    } else if (op_code == 0xC)  /* SW 110110 */
    {
    } else if (op_code == 0xC)  /* LWI 111010 */
    {
    } else if (op_code == 0xC)  /* SWI 111110 */
    {
    } else if (op_code == 0xC)  /* NOP 001100 */
    {
    } else if (op_code == 0x2C)  /* IMM 101100 */
    {
        //TODO: don't know if it works: Fatal error: An IMM instruction should not be present in the .s file
        instr = parse_instruction(instruction, TYPE_B, instr, &reg->im);
        reg->im = instr->im;
        update_PC(reg,4);
    } else {
        printf("unknown istruction\n");
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
        printf("  intero: %d ",(res->im)); /* complemento 2 */
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

int32_t checkOverflow(int64_t n, bool *c) {
    if (n > INT32_MAX) 
    {
        *c = true;
        return INT32_MAX;
    } 
    *c = false;
    return (int32_t)n;
}

void update_PC(struct Registers *reg,int32_t n) 
{
    reg->pc = reg->pc + n/4;
}

bool cmp_opcode(int8_t a,int8_t b)
{
    if ((a & 0b0011111) == b)
        return true;
    return false;
}

bool cmp_reg(int8_t a,int8_t b) 
{
    if ((a & 0b0001111) == b)
        return true;
    return false;
}