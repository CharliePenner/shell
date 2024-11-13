#ifndef _JOBS_H_
#define _JOBS_H_

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <fcntl.h>

#include "parser.h"

static const int JOBLIST_SIZE = 10;	// maximum number of background processes is 10

typedef struct {
        pid_t pid;
        char *cmd;
} job;

void ext_com(tokenlist *tokens, const char *input, job *joblist, char *path);

job *new_joblist();

void add_job(job *joblist, const pid_t pid, const char *cmd);

void check_jobs(job *joblist);

void del_job(job *joblist, int index);

#endif
