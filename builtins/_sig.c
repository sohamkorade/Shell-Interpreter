#include "_sig.h"

int _sig() {
  if (args_n != 3) return expect_args(2);

  job_t *job = get_job(atoi(args[1]));
  if (!job) {
    color_printf(red, "No job with the given number exists");
    return -1;
  }
  if (kill(job->pid, atoi(args[2])) < 0) {
    color_perror(red, "kill error");
    return -1;
  }
  return 0;
}