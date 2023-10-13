#include "headers.h"

char last_dir[MAX_LEN];
char launch_dir[MAX_LEN];
char temp_str[MAX_LEN];

char input[MAX_LEN];
char* args[MAX_ARGS];
int args_n = 0;
int switches[128];

char* history[MAX_STORE_HISTORY];

char* alias[MAX_ALIASES][2];
int alias_n = 0;

job_t jobs[MAX_JOBS];
int jobs_n = 0;

time_t seconds_elapsed;
