#ifndef __DECOMPILE_FILES_H
#define __DECOMPILE_FILES_H

struct file_data {
	char *filename;
	int buffer_length;
	int data_length;
	int *data;
};

int load_file(struct file_data **out, char *filename);
int load_int(struct file_data *f, char *input);
int resize_file_data(struct file_data *f);

#endif
