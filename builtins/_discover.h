#ifndef _discover_h
#define _discover_h

#include "../headers.h"

int _discover();
int _discover_dir(char* dir_name, char* filename, int optFiles, int optFolders,
                  int depth);

#endif