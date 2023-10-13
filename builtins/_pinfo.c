#include "_pinfo.h"

int _pinfo() {
  char proc_dir[MAX_LEN] = "";
  strcpy(proc_dir, "/proc/");
  strcat(proc_dir, args_n == 1 ? "self" : args[1]);

  strcpy(temp_str, proc_dir);
  strcat(temp_str, "/stat");
  FILE* f = fopen(temp_str, "r");
  if (f) {
    int pid, pgrp, tpgid;
    unsigned long memory;
    char pstatus;
    fscanf(f,
           "%d %*s %c %*d %d %*d %*d %d %*u %*u %*u %*u %*u %*u %*u %*d "
           "%*d %*d %*d %*d %*d %*u %lu",
           &pid, &pstatus, &pgrp, &tpgid, &memory);
    int foreground = pgrp == tpgid;
    printf(
        "PID: %d\n"
        "Process status: %c%c\n"
        "Virtual memory used: %lu bytes\n",
        pid, pstatus, foreground ? '+' : ' ', memory);
  } else {
    return -1;
  }

  strcpy(temp_str, proc_dir);
  strcat(temp_str, "/exe");
  char exec_path[MAX_LEN] = "";
  int exec_path_n = readlink(temp_str, exec_path, MAX_LEN);

  printf("Executable path: ");
  if (exec_path_n > 0) {
    exec_path[exec_path_n] = '\0';
    get_relative_path(exec_path, launch_dir);
    puts(exec_path);
  } else {
    color_printf(red, "(unable to access)\n");
  }

  return 0;
}