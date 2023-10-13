#ifndef utils_h
#define utils_h

#include "headers.h"

int expect_args(int n);

char* new_string();
int get_relative_path(char* path, char* relative_to);
void replace_str(char* s, char* find, char* replace);

void join_str2(char* s, char* s1, char* s2);
void join_str3(char* s, char* s1, char* s2, char* s3);

char* get_color(mode_t mode);

void remove_quotes_and_copy(char* new_s, char* old_s);
char* print_greeting();

void color_printf(char* color, char* fmt, ...);
void color_perror(char* color, char* s);

#endif