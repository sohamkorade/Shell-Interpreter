#include "runner.h"

int run(int forked) {
  if (!args_n) return 0;
  // for (int i = 0; i < args_n; i++) printf("<%s>\n", args[i]);
  struct timeval begin, end;
  gettimeofday(&begin, 0);

  int e = 0;
  // built-ins
  if (!strcmp(args[0], "bye") || !strcmp(args[0], "exit") ||
      !strcmp(args[0], "quit"))  // quit
    e = _quit();
  else if (!strcmp(args[0], "echo"))  // echo
    e = _echo();
  else if (!strcmp(args[0], "cd"))  // change directory
    e = _cd();
  else if (!strcmp(args[0], "pwd"))  // print working directory
    e = _pwd();
  else if (!strcmp(args[0], "pinfo"))  // process info
    e = _pinfo();
  else if (!strcmp(args[0], "ls"))  // list files and folders
    e = _ls();
  else if (!strcmp(args[0], "discover"))  // discover files and folders
    e = _discover();
  else if (!strcmp(args[0], "alias"))  // define or display aliases
    e = _alias();
  else if (!strcmp(args[0], "dalias"))  // remove existing aliases
    e = _dalias();
  else if (!strcmp(args[0], "history"))  // display history
    _history();
  else if (!strcmp(args[0], "jobs"))  // list background processes
    _jobs();
  else if (!strcmp(args[0], "help"))  // display help
    e = _help();
  else if (!strcmp(args[0], "sig"))  // send signal to job
    e = _sig();
  else if (!strcmp(args[0], "fg"))  // send job to foreground
    e = _fg();
  else if (!strcmp(args[0], "bg"))  // send job to background
    e = _bg();
  else if (!strcmp(args[0], "replay"))  // replay commands
    e = _replay();
  else if (forked)
    run_external();
  else
    e = run_fg();

  // exit when built-in commands are called via forked process
  if (forked) exit(e);

  gettimeofday(&end, 0);
  seconds_elapsed = end.tv_sec - begin.tv_sec;

  //   if (e < 0) {
  //     perror(args[0]);
  //   }
  return e;
}

int run_bg() {
  if (!args_n) return 0;
  // puts("running in bg");
  if (jobs_n >= MAX_JOBS) {
    color_printf(red, "max no. of background jobs reached\n");
    return -1;
  }

  int pid = fork();
  if (pid < 0)  // error
    color_perror(red, "fork error");
  else if (pid > 0) {  // original process
    job_t* new_job = add_job(pid);
    if (new_job)
      printf("[%d] %d '%s' started in background\n", new_job->job_id,
             new_job->pid, new_job->name);
  } else if (pid == 0) {  // forked process
    run(1);
  }
  return pid;
}

int run_fg() {
  if (!args_n) return 0;
  // puts("running in fg");
  // printf("%d == %d == %d\n", getpgid(0), getpid(), shell_pid);
  int pid = fork();
  if (pid < 0) {  // error
    color_perror(red, "fork error");
  } else if (pid > 0) {  // original process
    give_control_to_process(pid);
    int status = wait_for_process(pid);
    if (isatty(STDOUT_FILENO)) puts("");
    seconds_elapsed = 0;
    if (WIFSTOPPED(status)) {
      job_t* new_job = add_job(pid);
      if (new_job) {
        new_job->state = 0;  // stopped
        color_printf(yellow, "\n[%d] %d '%s' sent to background\n",
                     new_job->job_id, new_job->pid, new_job->name);
      } else {
        color_printf(red,
                     "\n[%d] %d '%s' not sent to background as max no. of "
                     "background jobs reached\n",
                     new_job->job_id, new_job->pid, new_job->name);
      }
      // } else if (WEXITSTATUS(status) == EXIT_FAILURE) {
      //   puts("");
      //   return -1;
    }
  } else if (pid == 0) {  // forked process
    run(1);
  }
  return pid;
}

void run_external() {
  setpgid(0, 0);
  args[args_n] = NULL;
  if (execvp(args[0], args) < 0)
    color_printf(red, "%s: invalid command\n", args[0]);
  exit(EXIT_FAILURE);
}
