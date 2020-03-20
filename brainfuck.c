#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char* brainfuck_to_c(const char* source);
int check_errors(const char* source, long* len);
char* simplify_source(const char* source, long* len);
int find_useless_commands(char* source, long* len);
char* translate(char* s);
void count_commands(char* s, long* i, long* j, char sign);
void indentation(char* buff, long long* buff_len, long* spaces);

char* brainfuck_to_c(const char* source) {
    long len = strlen(source);

    if (check_errors(source, &len)) {
    	char* error = malloc(sizeof(char) * 7);
    	memcpy(error, "Error!", 7);
    	return error;
    }

    char* s = simplify_source(source, &len);
    char* result = translate(s);

    free(s);
    return result;
}

int check_errors(const char* source, long* len) {
    int count = 0;
    for (int i = 0; i < *len; ++i) {
        count += source[i] == '[' ? 1 : (source[i] == ']' ? -1 : 0);
        if (count < 0) {
            return count;
        }
    }
    return count;
}

char* simplify_source(const char* source, long* len) {
    /* Replacing useless comands with '?' */
	
    int temp_len = 0;
    long result_len = *len;
    char* temp = malloc((*len + 1) * sizeof(char));
    char* result;

    memcpy(temp, source, *len + 1);
    
    for (int i = 0; i < *len; ++i) {
        if (temp[i] == ' ') {
            temp[i] = '?';
        }
    }
    
    do {
        result_len -= temp_len = find_useless_commands(temp, len);
    } while (temp_len);

    result = malloc((result_len + 1) * sizeof(char));

    for (int i = 0, j = 0; i <= *len; ++i) {
        if (temp[i] != '?') {
            result[j++] = temp[i];
        }
    }

    free(temp);
    return result;
}

int find_useless_commands(char* source, long* len) {
    /* Searching useless command sequences */
	
    long temp_len = 0;
    char* patterns[5] = {"+-", "-+", "><", "<>", "[]"};
    
    for (long i = 0; i < *len; ++i) {
        if (source[i] != '?') {
            for (int j = 0; j < 5; ++j) {
                if (source[i] == patterns[j][0]) {
                    for (long k = i + 1; k < *len; ++k) {
                        if (source[k] == patterns[j][1]) {
                            source[i] = source[k] = '?';
                            temp_len += 2;
                            break;
                        }
                        if (source[k] != '?') {
                            break;
                        }
                    }
                }
            }
        }
    }

    return temp_len;
}

char* translate(char* s) {
    char* buff = malloc(100000000 * sizeof(char));
    long long buff_len = 0;
    long i = 0, spaces = 0, j = 0;
    long len = strlen(s);
    char sign;
    buff[0] = '\0';

    while (i < len) {
        switch (s[i]) {
            case '+':
            case '-':
            		j = 0;
                    sign = s[i];
                    count_commands(s, &i, &j, s[i]);
                    if (spaces) indentation(buff, &buff_len, &spaces);
                    sprintf(buff + buff_len, "*p %c= %ld;\n", sign, j);
                    buff_len = buff_len + 9 + floor(log10(j));
                    continue;
            case '>':
            case '<':
            		j = 0;
                    sign = s[i] == '>' ? '+' : '-';
                    count_commands(s, &i, &j, s[i]);
                    if (spaces) indentation(buff, &buff_len, &spaces);
                    sprintf(buff + buff_len, "p %c= %ld;\n", sign, j);
                    buff_len = buff_len + 8 + floor(log10(j));
                    continue;
            case '.':
                    if (spaces) indentation(buff, &buff_len, &spaces);
                    sprintf(buff + buff_len, "putchar(*p);\n");
                    buff_len = buff_len + 13;
                    break;
            case ',':
                    if (spaces) indentation(buff, &buff_len, &spaces);
	                sprintf(buff + buff_len, "*p = getchar();\n");
                    buff_len = buff_len + 16;
                    break;
            case '[':
                    if (spaces) indentation(buff, &buff_len, &spaces);
                    sprintf(buff + buff_len, "if (*p) do {\n");
                    buff_len = buff_len + 13;
                    spaces += 2;
                    break;
            case ']':
                    spaces -= 2;
                    if (spaces) indentation(buff, &buff_len, &spaces);
                    sprintf(buff + buff_len, "} while (*p);\n");
                    buff_len = buff_len + 14;
                    break;
        }
    ++i;
    }

    return buff;
}

void indentation(char* buff, long long* buff_len, long* spaces) {
    /* Add indent */
	
    memset(buff + *buff_len, ' ', *spaces);
    *buff_len += *spaces;
}

void count_commands(char* s, long* i, long* j, char sign) {
    /* Counting consecutive same commands */
	
    while (s[*i] == sign) ++(*i), ++(*j);
}
