
#include <stdio.h>
#include <stdlib.h>

#include "core.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Wrong number of arguments\n");
        exit(1);
    }

    load_rom(argv[1]);
    while (1) {
        exec_op();
    }
}
