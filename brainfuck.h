#ifndef BRAINFUCK_TO_C
#define BRAINFUCK_TO_C

char* brainfuck_to_c(const char* source);
int check_errors(const char* source, long* len);
char* simplify_source(const char* source, long* len);
int find_useless_commands(char* source, long* len);
char* translate(char* s);
void count_commands(char* s, long* i, long* j, char sign);
void indentation(char* buff, long long* buff_len, long* spaces);

#endif
