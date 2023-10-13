#include "jobs.h"

job_t *get_job_by_pid(int pid) {
  for (int i = 0; i < jobs_n; i++) {
    if (jobs[i].pid == pid) {
      return &jobs[i];
      break;
    }
  }
  return NULL;
}

job_t *get_job(int job_id) {
  for (int i = 0; i < jobs_n; i++) {
    if (jobs[i].job_id == job_id) {
      return &jobs[i];
      break;
    }
  }
  return NULL;
}

void child_stopped() {
  int status;
  int pid = waitpid(-1, &status, WNOHANG);
  if (pid <= 0) return;
  // if (WIFSTOPPED(status)) return;
  // if (WIFEXITED(status)) return;
  job_t *stopped_job = get_job_by_pid(pid);
  if (stopped_job) {
    printf("\n[%d] %d '%s' exited ", stopped_job->job_id, pid,
           stopped_job->name);
    if (WIFEXITED(status))
      color_printf(green, "normally\n");
    else
      color_printf(red, "abnormally\n");
    // // slide up
    // for (int i = job_count - 1; i > bg_process_pos; i--) {
    //   strcpy(history[i - 1], history[i]);
    // }
    // job_count--;
    // bg_process_pid[job_count] = 0;
    // free(bg_process_name[job_count]);
    stopped_job->state = 0;  // stopped
  }
  puts("");
  show_prompt(input);
}

void ctrl_c() {
  color_printf(yellow, "^C\n");
  show_prompt("");
}
void ctrl_z() {
  color_printf(yellow, "^Z\n");
  show_prompt("");
}

job_t *add_job(int pid) {
  if (jobs_n >= MAX_JOBS) return NULL;

  job_t *new_job = &jobs[jobs_n];
  new_job->name = strdup(args[0]);
  for (int i = 1; i < args_n; i++) {
    strcat(new_job->name, " ");
    strcat(new_job->name, args[i]);
  }
  new_job->job_id = ++jobs_n;
  new_job->pid = pid;
  new_job->state = 1;  // running
  return new_job;
}

void free_jobs() {
  for (int i = 0; i < jobs_n; i++) free(jobs[i].name);
}

int _bg() {
  if (args_n != 2) return expect_args(1);
  job_t *job = get_job(atoi(args[1]));
  if (!job) {
    color_printf(red, "No job with the given number exists");
    return -1;
  }
  if (kill(job->pid, SIGCONT) < 0) {
    color_perror(red, "bg: cannot continue job");
    return -1;
  } else
    job->state = 1;  // running
  return 0;
}

int _fg() {
  if (args_n != 2) return expect_args(1);
  job_t *job = get_job(atoi(args[1]));
  if (!job) {
    color_printf(red, "No job with the given number exists");
    return -1;
  }
  own_process(job->pid);
  give_control_to_process(job->pid);
  if (kill(job->pid, SIGCONT) < 0)  // try to continue job
    color_perror(red, "fg: cannot continue job");
  else
    job->state = 1;  // running
  return wait_for_process(job->pid);
}

int own_process(int pid) { return setpgid(pid, getpid()); }

int give_control_to_process(int pid) {
  signal(SIGTTIN, SIG_IGN), signal(SIGTTOU, SIG_IGN);
  return tcsetpgrp(STDIN_FILENO, pid);
}

int wait_for_process(int pid) {
  int status;
  waitpid(pid, &status, WUNTRACED);

  tcsetpgrp(STDIN_FILENO, getpid());
  signal(SIGTTIN, SIG_DFL), signal(SIGTTOU, SIG_DFL);
  // return WIFEXITED(status) ? pid : -pid;
  return status;
}

int job_compare(const void *p1, const void *p2) {
  return strcmp(((const job_t *)p1)->name, ((const job_t *)p2)->name);
}

int _jobs() {
  qsort(jobs, jobs_n, sizeof(job_t), job_compare);
  for (int i = 0; i < jobs_n; i++) {
    if (!switches['s'] && !switches['r'] ||
        switches['s'] && jobs[i].state == 0 ||
        switches['r'] && jobs[i].state == 1) {
      printf("[%d]  %d  ", jobs[i].job_id, jobs[i].pid);
      if (jobs[i].state == 0)
        color_printf(red, "stopped");
      else
        color_printf(green, "running");
      printf("  %s\n", jobs[i].name);
    }
  }
  return 0;
}

void register_signal_handlers() {
  signal(SIGCHLD, child_stopped);
  signal(SIGTSTP, ctrl_z);
  signal(SIGINT, ctrl_c);
}