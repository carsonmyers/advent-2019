#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "decompile.h"

int main(int argc, char *argv[]) {
	printf("Arguments:\n");
	for (int i = 1; i < argc; i++) {
		printf(" > \"%s\"\n", argv[i]);
	}
	printf("\n");

	struct program_args *args = new_program_args();
	int result = parse_args(args, argc, argv);
	if (result != 0) {
		exit(result);
	}

	printf("Input: %s\n", args->input);
	printf("Output: %s\n", args->output);
	printf("Optimize? %s\n", args->optimize ? "yes" : "no");
	printf("Expand? %s\n", args->expand_unused ? "yes" : "no");
	printf("Hex? %s\n", args->hex_addressing ? "yes" : "no");

	return 0;
}
