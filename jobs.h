#ifndef jobs_h
#define jobs_h

#include "headers.h"
#include "prompt.h"

job_t *get_job_by_pid(int pid);
job_t *get_job(int job_id);

int _jobs();

void child_stopped();
void ctrl_c();
void ctrl_z();
void register_signal_handlers();

job_t *add_job(int pid);
void free_jobs();

int _fg();
int _bg();

int own_process(int pid);
int give_control_to_process(int pid);
int wait_for_process(int pid);

#endif