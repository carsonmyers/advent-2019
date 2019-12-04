#ifndef __DECOMPILE_ARGS_H
#define __DECOMPILE_ARGS_H

#include <stdbool.h>

struct program_args {
	char *input;
	char *output;
	bool optimize;
	bool expand_unused;
	bool hex_addressing;
};

int parse_args(struct program_args **out, int argc, char *argv[]);
int parse_arg(struct program_args *out, int *current_arg, int argc, char *argv[]);
int parse_switch(struct program_args *out, int *current_arg, int argc, char *argv[]);
void split_value(char **arg, char **value);

#endif
