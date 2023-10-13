#ifndef runner_h
#define runner_h

#include "builtins.h"
#include "headers.h"
#include "history.h"
#include "jobs.h"
#include "prompt.h"

int run(int forked);
int run_fg();
int run_bg();
void run_external();

#endif