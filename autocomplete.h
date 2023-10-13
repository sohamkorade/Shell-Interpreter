#ifndef _autocomplete_h
#define _autocomplete_h

#include "headers.h"

void suggest(char* input);
void get_autocomplete(char* input, char* autocompleted, int pos);
void fill_autocomplete(char* input, char* autocomplete);

#endif