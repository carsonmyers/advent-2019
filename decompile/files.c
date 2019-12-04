#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "files.h"

int load_file(struct file_data **out, char *filename) {
	*out = malloc(sizeof(struct file_data));
	if (*out == NULL) {
		fprintf(stderr, "Out of memory\n");
		return 1;
	}

	memset(*out, 0, sizeof(struct file_data));
	(*out)->filename = filename;

	FILE *f = fopen(filename, "r");
	if (f == NULL) {
		fprintf(stderr, "Error opening file %s: %s\n", filename, strerror(errno));
		return 1;
	}

	long int size = 0;
	fseek(f, 0L, SEEK_END);
	size = ftell(f);
	fseek(f, 0L, SEEK_SET);

	char *data = (char *)malloc(size);
	if (size != fread(data, sizeof(char), size, f)) {
		free(data);
		fprintf(stderr, "Error loading fine %s: %s\n", filename, strerror(errno));
		return 1;
	}
	fclose(f);

	(*out)->data_length = 256;
	(*out)->program = malloc(sizeof(uint8_t) * (*out)->data_length);
	if ((*out)->program == NULL) {
		free(data);
		fprintf(stderr, "Out of memory\n");
		return 1;
	}

	int next_num = -1;
	for (int i = 0; i < size; i++) {
		if (next_num < 0) {
			next_num = i;
		}

		if (data[i] == ',') {
			data[i] = 0;
			int err = load_instruction(*out, &data[next_num]);
			if (err != 0) {
				free(data);
				free((*out)->program);
				return err;
			}
			next_num = -1;
		}
	}

	if (next_num > 0 && data[next_num] != 0) {
		int err = load_instruction(*out, &data[next_num]);
		if (err != 0) {
			free(data);
			free((*out)->program);
			return err;
		}
	}

	return 0;
}

int load_instruction(struct file_data *f, char *input) {
	int idx = f->program_length + 1;
	if (idx == f->data_length) {
		int err = resize_file_data(f);
		if (err != 0) {
			return err;
		}
	}

	f->program[idx] = atoi(input);
	f->program_length++;

	return 0;
}

int resize_file_data(struct file_data *f) {
	int *old_program = f->program;

	int next_size = f->data_length * 2;
	f->program = malloc(next_size * sizeof(int));
	if (f->program == NULL) {
		fprintf(stderr, "Out of memory\n");
		return 1;
	}

	for (int i = 0; i < f->program_length; i++) {
		f->program[i] = old_program[i];
	}

	free(old_program);
	return 0;
}
