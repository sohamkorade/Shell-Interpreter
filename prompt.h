#ifndef prompt_h
#define prompt_h

#include "autocomplete.h"
#include "headers.h"
#include "history.h"

int show_prompt(char* input);
int get_history(int history_pos, int up);
void handle_input();

#endif