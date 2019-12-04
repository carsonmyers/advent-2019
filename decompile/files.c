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

	(*out)->buffer_length = 256;
	(*out)->data = malloc(sizeof(uint8_t) * (*out)->data_length);
	if ((*out)->data == NULL) {
		free(data);
		free(*out);
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
			int err = load_int(*out, &data[next_num]);
			if (err != 0) {
				free(data);
				free((*out)->data);
				free(*out);
				return err;
			}
			next_num = -1;
		}
	}

	if (next_num > 0 && data[next_num] != 0) {
		int err = load_int(*out, &data[next_num]);
		if (err != 0) {
			free(data);
			free((*out)->data);
			free(*out);
			return err;
		}
	}

	return 0;
}

int load_int(struct file_data *f, char *input) {
	int idx = f->data_length + 1;
	if (idx == f->buffer_length) {
		int err = resize_file_data(f);
		if (err != 0) {
			return err;
		}
	}

	f->data[idx] = atoi(input);
	f->data_length++;

	return 0;
}

int resize_file_data(struct file_data *f) {
	int *old_data = f->data;

	int next_size = f->buffer_length * 2;
	f->data = malloc(next_size * sizeof(int));
	if (f->data == NULL) {
		fprintf(stderr, "Out of memory\n");
		return 1;
	}

	for (int i = 0; i < f->data_length; i++) {
		f->data[i] = old_data[i];
	}

	free(old_data);
	return 0;
}
