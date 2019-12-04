#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "files.h"
#include "program.h"

int load_program(struct program **out, struct file_data *f) {
	*out = malloc(sizeof(struct program));
	if (*out == NULL) {
		fprintf(stderr, "Out of memory\n");
		return 1;
	}

	memset(*out, 0, sizeof(struct program));
	(*out)->file = f;
	(*out)->buffer_length = 256;
	(*out)->program = malloc((*out)->buffer_length * sizeof(struct ix));
	if ((*out)->program == NULL) {
		free(*out);
		fprintf(stderr, "Out of memory\n");
		return 1;
	}

	int idx_file = 0;
	while (idx_file < f->data_length) {
		int err = load_instruction(*out, f, &idx_file);
		if (err != 0) {
			free((*out)->program);
			free(*out);
			return err;
		}
	}

	return 0;
}

int load_instruction(struct program *p, struct file_data *f, int *idx) {
	if (p->program_length >= p->buffer_length - 1) {
		int err = resize_program_buffer(p);
		if (err != 0) {
			return err;
		}
	}
	if (*idx > UINT16_MAX) {
		fprintf(stderr, "Exceeded max addressable space of %d\n", UINT16_MAX);
		return 1;
	}

	int opcode = f->data[*idx];
	if (opcode > UINT8_MAX) {
		fprintf(stderr, "Illegal instruction %d at index %d\n", opcode, *idx);
		return 1;
	}

	struct ix ix = {(uint8_t)opcode, 0, (uint16_t)*idx};
	switch (opcode) {
	case IXADD:
		ix.data.add = (struct ix_add){};
		if (ix_len(&ix.len, 4, f, *idx) != 0) {
			return 1;
		}
		if (addr(&ix.data.add.src_a, f->data[*idx + 1], *idx) != 0) {
			return 1;
		}
		if (addr(&ix.data.add.src_b, f->data[*idx + 2], *idx) != 0) {
			return 1;
		}
		if (addr(&ix.data.add.dst, f->data[*idx + 3], *idx) != 0) {
			return 1;
		}
		break;
	case IXMUL:
		ix.data.mul = (struct ix_mul){};
		if (ix_len(&ix.len, 4, f, *idx) != 0) {
			return 1;
		}
		if (addr(&ix.data.mul.src_a, f->data[*idx + 1], *idx) != 0) {
			return 1;
		}
		if (addr(&ix.data.mul.src_b, f->data[*idx + 2], *idx) != 0) {
			return 1;
		}
		if (addr(&ix.data.mul.dst, f->data[*idx + 3], *idx) != 0) {
			return 1;
		}
		break;
	case IXTERM:
		ix.data.term = (struct ix_term){};
		ix.len = 1;
		break;
	default:
		fprintf(stderr, "Illegal instruction %d at index %d\n", opcode, *idx);
	}
	return 0;
}

int ix_len(uint8_t *out, int len, struct file_data *f, int idx) {
	if (f->data_length - 1 - idx < len) {
		fprintf(stderr, "ADD at index %d missing data, expected %d\n", idx, len);
		return 1;
	}

	*out = (uint8_t)len;

	return 0;
}

int addr(uint16_t *out, int data, int idx) {
	if (data < 0 || data > UINT16_MAX) {
		fprintf(stderr, "Invalid address %d at index %d\n", data, idx);
		return 1;
	}

	*out = (uint16_t)data;

	return 0;
}
