struct file_data {
	char *filename;
	int data_length;
	int program_length;
	int *program;
};

int load_file(struct file_data **out, char *filename);
int load_instruction(struct file_data *f, char *input);
int resize_file_data(struct file_data *f);
