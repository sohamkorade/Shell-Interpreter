#include "_replay.h"

int _replay() {
  color_printf(yellow, "WARNING: EXPERIMENTAL!\n");
  // replay <part 0> -command <part 1> -interval <part 2> -period <part 3>
  int part = 0, interval = 0, period = 0;
  char cmd[MAX_LEN] = "";
  for (int i = 1; i < args_n; i++) {
    if (!strcmp(args[i], "-command")) {
      part = 1;
      continue;
    } else if (!strcmp(args[i], "-interval")) {
      part = 2;
      continue;
    } else if (!strcmp(args[i], "-period")) {
      part = 3;
      continue;
    }
    if (part == 1) {
      strcat(cmd, args[i]);
      strcat(cmd, " ");
    } else if (part == 2) {
      interval = atoi(args[i]);
    } else if (part == 3) {
      period = atoi(args[i]);
    }
  }

  // printf("cmd=%s i=%d p=%d\n", cmd, interval, period);

  if (cmd[0] * interval * period == 0) {
    color_printf(red, "replay: invalid format\n");
    return -1;
  }

  int times = period / interval;
  int pid = fork();
  if (pid < 0)  // error
    color_perror(red, "replay: fork error");
  else if (pid > 0) {  // original process
    // nothing to do
  } else if (pid == 0) {  // forked process
    char* cmd2 = strdup(cmd);
    while (times--) {
      puts("");
      interpret(cmd2);
      show_prompt("");
      sleep(interval);
    }
    free(cmd2);
    exit(EXIT_SUCCESS);
  }
  return pid;
}