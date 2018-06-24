
#include<stdbool.h>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>

typedef enum OpKind {
    OP_CLEAR,
    OP_RETURN,
    OP_JUMP,
    OP_CALL,
    OP_SKIP_EQ_VAL,
    OP_SKIP_NE_VAL,
    OP_SKIP_EQ_REG,
    OP_LOAD_VAL,
    OP_ADD_VAL,
    OP_LOAD_REG,
    OP_OR,
    OP_AND,
    OP_XOR,
    OP_ADD_REG,
    OP_SUB,
    OP_SHIFT_R,
    OP_SUBN,
    OP_SHIFT_L,
    OP_SKIP_NE_REG,
    OP_LOAD_ADDR,
    OP_JUMP_OFFSET,
    OP_RANDOM,
    OP_DRAW,
    OP_SKIP_KEY_PRESSED,
    OP_SKIP_KEY_NOT_PRESSED,
    OP_LOAD_DELAY,
    OP_WAIT_KEY_PRESS,
    OP_SET_DELAY,
    OP_SET_SOUND,
    OP_ADD_I,
    OP_LOAD_FONT,
    OP_LOAD_BCD,
    OP_LOAD_REGS,
    OP_SET_REGS,
} OpKind;

uint16_t stack[16];
uint16_t i_reg;
uint16_t pc = 512;
uint8_t gen_regs[16];
uint8_t memory[4096];

void load_rom(char *path) {
    FILE *fp = fopen(path, "rb");

    if (fp == NULL) {
        printf("Error opening ROM file\n");
        exit(1);
    }

    fread(memory + pc, 1, 4096, fp);

    fclose(fp);
}

void get_op_info(OpKind *kind, int *nnn, int *n, int *x, int *y, int *kk) {
    uint16_t op = (memory[pc] << 8) + memory[pc+1];

    *nnn = op & 0xFFF;          // lowest 12 bits of instruction
    *n   = op & 0xF;            // lowest 4 bits of instruction
    *x   = (op >> 8) & 0xF;     // lower 4 bits of the high byte of instruction
    *y   = (op >> 4) & 0xF;     // upper 4 bits of the low byte of instruction
    *kk  = op & 0xFF;           // lowest 8 bits of instruction

    // 00E0
    if (op == 0x00E0) {
        *kind = OP_CLEAR;
        return;
    }
    // 00EE
    if (op == 0x00EE) {
        *kind = OP_RETURN;
        return;
    }
    // 1NNN
    if (op >> 12 == 0x1) {
        *kind = OP_JUMP;
        return;
    }
    // 2NNN
    if (op >> 12 == 0x2) {
        *kind = OP_CALL;
        return;
    }
    // 3XNN
    if (op >> 12 == 0x3) {
        *kind = OP_SKIP_EQ_VAL;
        return;
    }
    // 4XNN
    if (op >> 12 == 0x4) {
        *kind = OP_SKIP_NE_VAL;
        return;
    }
    // 5XY0
    if (op >> 12 == 0x5) {
        *kind = OP_SKIP_EQ_REG;
        return;
    }
    // 6XNN
    if (op >> 12 == 0x6) {
        *kind = OP_LOAD_VAL;
        return;
    }
    // 7XNN
    if (op >> 12 == 0x7) {
        *kind = OP_ADD_VAL;
        return;
    }
    // 8XY0
    if (op >> 12 == 0x8 && (op & 0xF) == 0x0) {
        *kind = OP_LOAD_REG;
        return;
    }
    // 8XY1
    if (op >> 12 == 0x8 && (op & 0xF) == 0x1) {
        *kind = OP_OR;
        return;
    }
    // 8XY2
    if (op >> 12 == 0x8 && (op & 0xF) == 0x2) {
        *kind = OP_AND;
        return;
    }
    // 8XY3
    if (op >> 12 == 0x8 && (op & 0xF) == 0x3) {
        *kind = OP_XOR;
        return;
    }
    // 8XY4
    if (op >> 12 == 0x8 && (op & 0xF) == 0x4) {
        *kind = OP_ADD_REG;
        return;
    }
    // 8XY5
    if (op >> 12 == 0x8 && (op & 0xF) == 0x5) {
        *kind = OP_SUB;
        return;
    }
    // 8XY6
    if (op >> 12 == 0x8 && (op & 0xF) == 0x6) {
        *kind = OP_SHIFT_R;
        return;
    }
    // 8XY7
    if (op >> 12 == 0x8 && (op & 0xF) == 0x7) {
        *kind = OP_SUBN;
        return;
    }
    // 8XYE
    if (op >> 12 == 0x8 && (op & 0xF) == 0xE) {
        *kind = OP_SHIFT_L;
        return;
    }
    // 9XY0
    if (op >> 12 == 0x9) {
        *kind = OP_SKIP_NE_REG;
        return;
    }
    // ANNN
    if (op >> 12 == 0xA) {
        *kind = OP_LOAD_ADDR;
        return;
    }
    // BNNN
    if (op >> 12 == 0xB) {
        *kind = OP_JUMP_OFFSET;
        return;
    }
    // CXNN
    if (op >> 12 == 0xC) {
        *kind = OP_RANDOM;
        return;
    }
    // DXYN
    if (op >> 12 == 0xD) {
        *kind = OP_DRAW;
        return;
    }
    // EX9E
    if (op >> 12 == 0xE && (op & 0xFF) == 0x9E) {
        *kind = OP_SKIP_KEY_PRESSED;
        return;
    }
    // EXA1
    if (op >> 12 == 0xE && (op & 0xFF) == 0xA1) {
        *kind = OP_SKIP_KEY_NOT_PRESSED;
        return;
    }
    // FX07
    if (op >> 12 == 0xF && (op & 0xFF) == 0x07) {
        *kind = OP_LOAD_DELAY;
        return;
    }
    // FX0A
    if (op >> 12 == 0xF && (op & 0xFF) == 0x0A) {
        *kind = OP_WAIT_KEY_PRESS;
        return;
    }
    // FX15
    if (op >> 12 == 0xF && (op & 0xFF) == 0x15) {
        *kind = OP_SET_DELAY;
        return;
    }
    // FX18
    if (op >> 12 == 0xF && (op & 0xFF) == 0x18) {
        *kind = OP_SET_SOUND;
        return;
    }
    // FX1E
    if (op >> 12 == 0xF && (op & 0xFF) == 0x1E) {
        *kind = OP_ADD_I;
        return;
    }
    // FX29
    if (op >> 12 == 0xF && (op & 0xFF) == 0x29) {
        *kind = OP_LOAD_FONT;
        return;
    }
    // FX33
    if (op >> 12 == 0xF && (op & 0xFF) == 0x33) {
        *kind = OP_LOAD_BCD;
        return;
    }
    // FX55
    if (op >> 12 == 0xF && (op & 0xFF) == 0x55) {
        *kind = OP_LOAD_REGS;
        return;
    }
    // FX65
    if (op >> 12 == 0xF && (op & 0xFF) == 0x65) {
        *kind = OP_SET_REGS;
        return;
    }

    printf("Op not recognized\n");
    exit(1);
}

void op_not_implemented(OpKind op) {
    printf("Opcode not implemented: %d\n", op);
    exit(1);
}

void exec_op() {
    int nnn = 0;
    int n = 0;
    int x = 0;
    int y = 0;
    int kk = 0;
    OpKind op;

    get_op_info(&op, &nnn, &n, &x, &y, &kk);

    switch(op) {
        case OP_CLEAR:
            op_not_implemented(op);
            break;
        case OP_RETURN:
            op_not_implemented(op);
            break;
        case OP_JUMP:
            op_not_implemented(op);
            break;
        case OP_CALL:
            op_not_implemented(op);
            break;
        case OP_SKIP_EQ_VAL:
            op_not_implemented(op);
            break;
        case OP_SKIP_NE_VAL:
            op_not_implemented(op);
            break;
        case OP_SKIP_EQ_REG:
            op_not_implemented(op);
            break;
        case OP_LOAD_VAL:
            op_not_implemented(op);
            break;
        case OP_ADD_VAL:
            op_not_implemented(op);
            break;
        case OP_LOAD_REG:
            op_not_implemented(op);
            break;
        case OP_OR:
            op_not_implemented(op);
            break;
        case OP_AND:
            op_not_implemented(op);
            break;
        case OP_XOR:
            op_not_implemented(op);
            break;
        case OP_ADD_REG:
            op_not_implemented(op);
            break;
        case OP_SUB:
            op_not_implemented(op);
            break;
        case OP_SHIFT_R:
            op_not_implemented(op);
            break;
        case OP_SUBN:
            op_not_implemented(op);
            break;
        case OP_SHIFT_L:
            op_not_implemented(op);
            break;
        case OP_SKIP_NE_REG:
            op_not_implemented(op);
            break;
        case OP_LOAD_ADDR:
            op_not_implemented(op);
            break;
        case OP_JUMP_OFFSET:
            op_not_implemented(op);
            break;
        case OP_RANDOM:
            op_not_implemented(op);
            break;
        case OP_DRAW:
            op_not_implemented(op);
            break;
        case OP_SKIP_KEY_PRESSED:
            op_not_implemented(op);
            break;
        case OP_SKIP_KEY_NOT_PRESSED:
            op_not_implemented(op);
            break;
        case OP_LOAD_DELAY:
            op_not_implemented(op);
            break;
        case OP_WAIT_KEY_PRESS:
            op_not_implemented(op);
            break;
        case OP_SET_DELAY:
            op_not_implemented(op);
            break;
        case OP_SET_SOUND:
            op_not_implemented(op);
            break;
        case OP_ADD_I:
            op_not_implemented(op);
            break;
        case OP_LOAD_FONT:
            op_not_implemented(op);
            break;
        case OP_LOAD_BCD:
            op_not_implemented(op);
            break;
        case OP_LOAD_REGS:
            op_not_implemented(op);
            break;
        case OP_SET_REGS:
            op_not_implemented(op);
            break;
        default:
            printf("OP not handled: %d\n", op);
            exit(1);
            break;
    }
}
