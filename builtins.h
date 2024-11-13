#ifndef _BUILTINS_H_
#define _BUILTINS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "parser.h"
#include "jobs.h"

void shell_exit(char prev[3][200], job*);
bool shell_cd(char*);
void shell_echo(tokenlist*);
void shell_jobs(job*);

#endif
