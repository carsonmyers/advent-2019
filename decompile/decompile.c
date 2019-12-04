#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "decompile.h"

int main(int argc, char *argv[]) {
	struct program_args *args;
	int result = parse_args(&args, argc, argv);
	if (result != 0) {
		exit(result);
	}

	return 0;
}
