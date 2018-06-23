
#include<stdbool.h>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>

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

    fread(memory, 1, 4096, fp);

    fclose(fp);
}
