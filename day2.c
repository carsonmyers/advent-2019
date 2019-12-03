#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day2.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("No input files\n");
        exit(0);
    }

    int exit_code = 0;
    for (int i = 1; i < argc; i++) {
        int *program = load_program(argv[i]);
        int result = run_program(program);
        if (result < 0) {
            fprintf(stderr, "Program %s terminated abnormally\n", argv[i]);
            exit_code = 2;
        } else {
            printf("Program %s terminated with final value %d\n", argv[i], result);
        }
    }

    return exit_code;
}

int *load_program(char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "Error loading file %s: %s\n", filename, strerror(errno));
        exit(1);
    }

    long int size = 0;
    fseek(f, 0L, SEEK_END);
    size = ftell(f);
    fseek(f, 0L, SEEK_SET);

    char *data = (char *)malloc(size + 1);
    if (size != fread(data, sizeof(char), size, f)) {
        free(data);
        fprintf(stderr, "Error reading file %s: %s\n", filename, strerror(errno));
        exit(1);
    }

    fclose(f);
    data[size] = 0;

    int *program = (int *)malloc(265 * sizeof(int) + 2);
    if (program == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }

    program[0] = 256;
    program[1] = 0;

    int idx = -1;
    for (int i = 0; i < size; i++) {
        if (idx < 0) {
            idx = i;
        }

        if (data[i] == ',') {
            data[i] = 0;
            load_instruction(&program, &data[idx]);
            idx = -1;
        }
    }

    if (idx > 0 && data[idx] != 0) {
        load_instruction(&program, &data[idx]);
    }

    return program;
}

void load_instruction(int *program[], char *data) {
    int idx = (*program)[1] + 2;

    if (idx == (*program)[0] - 1) {
        resize_program(program);
    }


    (*program)[idx] = atoi(data);
    (*program)[1]++;
}

void resize_program(int *program[]) {
    int *pdata = *program;

    int next_size = pdata[0] * 2;
    int program_length = pdata[1];

    int *data = (int *)malloc(next_size * sizeof(int) + 2);
    if (data == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }

    for (int i = 0; i < pdata[0] + 2; i++) {
        data[i] = pdata[i];
    }
    data[0] = next_size;

    *program = data;
}

int run_program(int *program) {
    int length = program[1];
    int *pdata = &program[2];

    int idx = 0;
    int last_dst = 0;

    for (;;) {
        if (idx >= length) {
            fprintf(stderr, "Program not terminated\n");
            return -1;
        }

        int a, b = 0;

        printf("INST %d AT %d [%d %d %d]\n", pdata[idx], idx, pdata[idx + 1], pdata[idx + 2], pdata[idx + 3]);

        switch (pdata[idx]) {
        case IADD:
            a = pdata[pdata[idx + 1]];
            b = pdata[pdata[idx + 2]];
            pdata[pdata[idx + 3]] = a + b;
            last_dst = pdata[idx + 3];
            printf("ADD %d %d, STORE %d IN %d\n", a, b, a + b, last_dst);
            idx += 4;
            break;
        case IMUL:
            a = pdata[pdata[idx + 1]];
            b = pdata[pdata[idx + 2]];
            pdata[pdata[idx + 3]] = a * b;
            last_dst = pdata[idx + 3];
            printf("MUL %d %d, STORE %d IN %d\n", a, b, a * b, last_dst);
            idx += 4;
            break;
        case IEXIT:
            return pdata[last_dst];
            break;
        default:
            fprintf(stderr, "Illegal instruction %d\n", pdata[idx]);
            return -1;
            break;
        }
    }
}