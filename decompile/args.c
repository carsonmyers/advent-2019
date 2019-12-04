#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "args.h"

int parse_args(struct program_args **out, int argc, char *argv[]) {
	*out = malloc(sizeof(struct program_args));
	if (out == NULL) {
		fprintf(stderr, "Out of memory\n");
		return 1;
	}

	memset((void *)*out, 0, sizeof(struct program_args));

	int current_arg = 1;
	while (current_arg < argc) {
		int args_left = argc - current_arg;
		char **next_args = &argv[current_arg];

		int result = parse_arg(*out, &current_arg, args_left, next_args);
		if (result != 0) {
			return result;
		}
	}

	if ((*out)->input == NULL) {
		fprintf(stderr, "No input file specified\n");
		return 1;
	}

	if ((*out)->output == NULL) {
		(*out)->output = malloc(strlen((*out)->input) + 2);

		char *r = (*out)->input;
		char *w = (*out)->output;
		char *last_dot = NULL;
		while (*r != 0) {
			if (*r == '.') {
				last_dot = w;
			}

			*w++ = *r++;
		}

		if (last_dot == NULL) {
			last_dot = w;
		}

		last_dot[0] = '.';
		last_dot[1] = 'a';
		last_dot[2] = 0;
	}

	return 0;
}

int parse_arg(struct program_args *out, int *current_arg, int argc, char *argv[]) {

	char *arg = argv[0];
	if (arg[0] == '-') {
		int result = parse_switch(out, current_arg, argc, argv);
		if (result != 0) {
			return result;
		}
	} else {
		if (out->input != NULL) {
			fprintf(stderr, "Only one input file is supported\n");
			return 1;
		}

		out->input = arg;
		*current_arg += 1;
	}

	return 0;
}

int parse_switch(struct program_args *out, int *current_arg, int argc, char *argv[]) {
	char *arg = argv[0];
	int len = strlen(arg);
	if (len <= 1) {
		return 0;
	}

	if (arg[1] == '-') {
		if (len == 2) {
			return 0;
		}

		char *value = NULL;
		char *name = &arg[2];
		split_value(&name, &value);

		if (strcmp(name, "out") == 0) {
			if (out->output != NULL) {
				fprintf(stderr, "Only one output file is supported\n");
				return 1;
			}

			if (value != NULL) {
				out->output = value;
				*current_arg += 1;
				return 0;
			}

			if (argc <= 1) {
				fprintf(stderr, "--out switch must have an argument\n");
				return 1;
			}

			char *next_arg = argv[1];
			if (next_arg[0] == '-') {
				fprintf(stderr, "--out switch must have an argument\n");
				return 1;
			}

			out->output = next_arg;
			*current_arg += 2;
		} else if (strcmp(&arg[2], "optimize") == 0) {
			out->optimize = true;
			*current_arg += 1;
		} else if (strcmp(&arg[2], "expand-unused") == 0) {
			out->expand_unused = true;
			*current_arg += 1;
		} else if (strcmp(&arg[2], "hex-addresses") == 0) {
			out->hex_addressing = true;
			*current_arg += 1;
		} else {
			fprintf(stderr, "Invalid switch: %s\n", &arg[2]);
			return 1;
		}
	} else {
		for (int i = 1; i < len; i++) {
			switch (arg[i]) {
			case 'o':
				if (out->output != NULL) {
					fprintf(stderr, "Only one output file is supported\n");
					return 1;
				}

				if (i != len - 1 || argc <= 1) {
					fprintf(stderr, "-o switch must have an argument\n");
					return 1;
				}

				char *next_arg = argv[1];
				if (next_arg[0] == '-') {
					fprintf(stderr, "-o switch must have an argument\n");
					return 1;
				}

				out->output = next_arg;
				*current_arg += 1;
				break;
			case 'O':
				out->optimize = true;
				break;
			case 'x':
				out->expand_unused = true;
				break;
			case 'h':
				out->hex_addressing = true;
				break;
			default:
				fprintf(stderr, "Invalid switch: %c\n", arg[i]);
				return 1;
			}
		}

		*current_arg += 1;
	}

	return 0;
}

void split_value(char **arg, char **value) {
	char *r = *arg;
	do {
		if (*r == '=') {
			*r = 0;
			*value = r + 1;
			break;
		}
	} while (*r++ != 0);
}
