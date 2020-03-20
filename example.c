#include <stdio.h>
#include <stdlib.h>
#include "brainfuck.h"

int main() {
	char* s = brainfuck_to_c("++++<<--[+[++->>--]++--]>>++");
	
	printf("%s", s);
	
	free(s);
	return 0;
}
