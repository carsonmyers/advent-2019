#include <stdint.h>
#include "args.h"

#define IXADD 1
#define IXMUL 2
#define IXTERM 99

struct ix_base {
	uint8_t opcode;
	uint8_t len;
	uint16_t addr;
};

struct ix_add {
	struct ix_base base;
	uint16_t src_a;
	uint16_t src_b;
	uint16_t dst;
};

struct ix_mul {
	struct ix_base base;
	uint16_t src_a;
	uint16_t src_b;
	uint16_t dst;
};

struct ix_term {
	struct ix_base base;
};

enum ix {
	ix_add,
	ix_mul,
	ix_term,
};

struct program {
	struct program_args *args;
	uint16_t len;
	enum ix *program;
};
