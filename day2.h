#define IADD 1
#define IMUL 2
#define IEXIT 99

int *load_program(char *filename);
void load_instruction(int *program[], char *data);
void resize_program(int *program[]);
int run_program(int *program);