#ifndef __DECOMPILE_PROGRAM_H
#define __DECOMPILE_PROGRAM_H

#include <stdint.h>
#include "args.h"
#include "files.h"

#define IXADD 1
#define IXMUL 2
#define IXTERM 99

struct ix_add {
	uint16_t src_a;
	uint16_t src_b;
	uint16_t dst;
};

struct ix_mul {
	uint16_t src_a;
	uint16_t src_b;
	uint16_t dst;
};

struct ix_term { };

union ix_data {
	struct ix_add add;
	struct ix_mul mul;
	struct ix_term term;
};

struct ix {
	uint8_t opcode;
	uint8_t len;
	uint16_t addr;
	union ix_data data;
};

struct program {
	struct file_data *file;
	int buffer_length;
	int program_length;
	struct ix *program;
};

int load_program(struct program **out, struct file_data *f);
int load_instruction(struct program *p, struct file_data *f, int *idx);
int resize_program_buffer(struct program *p);
int ix_len(uint8_t *out, int len, struct file_data *f, int idx);
int addr(uint16_t *out, int data, int idx);

#endif
