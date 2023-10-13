#ifndef headers_h
#define headers_h

#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>

#include "utils.h"

#define MAX_LEN 500
#define MAX_ARGS 100
#define MAX_STORE_HISTORY 20
#define MAX_DISPLAY_HISTORY 10
#define MAX_JOBS 100
#define MAX_ALIASES 100
#define MAX_PIPES 100

#define red "\e[0;31m"
#define green "\e[0;32m"
#define yellow "\e[0;33m"
#define blue "\e[0;34m"
#define purple "\e[0;35m"
#define cyan "\e[0;36m"
#define white "\e[0m"

#define gray "\e[0;90m"

#define bold_red "\e[1;31m"
#define bold_green "\e[1;32m"
#define bold_yellow "\e[1;33m"
#define bold_blue "\e[1;34m"
#define bold_purple "\e[1;35m"
#define bold_cyan "\e[1;36m"
#define bold_white "\e[1m"

extern char last_dir[MAX_LEN];
extern char launch_dir[MAX_LEN];
extern char temp_str[MAX_LEN];

extern char input[MAX_LEN];
extern char* args[MAX_ARGS];
extern int args_n;
extern int switches[128];

extern char* history[MAX_STORE_HISTORY];

extern char* alias[MAX_ALIASES][2];
extern int alias_n;

typedef struct job_t {
  char* name;
  int job_id, pid, state;
} job_t;
extern job_t jobs[MAX_JOBS];
extern int jobs_n;

extern time_t seconds_elapsed;

#endif