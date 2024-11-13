#include "jobs.h"

void ext_com(tokenlist *tokens, const char *input, job *joblist, char *path)
{
	bool bg = false;
        if (tokens->items[tokens->size - 1][0] == '&') {
                bg = true;
                free(tokens->items[tokens->size - 1]);
                tokens->items[tokens->size - 1] = NULL;
                tokens->size--;
        }
	
	pid_t pid = fork();
	if (pid < 0) {
		perror("ext_com: unable to create child process");
	}
	else if (pid == 0) {		// child process
		int fd_in, fd_out, end_cmd = tokens->size;
		for (int i = 0; i < tokens->size; i++){
			if (tokens->items[i][0] == '>'){
				if (close(STDOUT_FILENO) < 0){
					perror("Error close()");
					exit(EXIT_FAILURE);
				}
				if ((fd_out = open(tokens->items[i+1], O_RDWR | O_CREAT, S_IWUSR | S_IRUSR)) < 0){
					perror("Error open()");
					exit(EXIT_FAILURE);
				}
				if (dup2(STDOUT_FILENO, fd_out) < 0){
					perror("Error dup2()");
					exit(EXIT_FAILURE);
				}
				if (i < end_cmd){
					end_cmd = i;
				}
			}
			else if (tokens->items[i][0] == '<'){
				if (close(STDIN_FILENO) < 0){
					perror("Error close()");
					exit(EXIT_FAILURE);
				}
				if ((fd_in = open(tokens->items[i+1], O_RDONLY, S_IWUSR | S_IRUSR)) < 0){
					perror("Error open()");
					exit(EXIT_FAILURE);
				}
				if (dup2(STDIN_FILENO, fd_in) < 0){
					perror("Error dup2()");
					exit(EXIT_FAILURE);
				}
				if (i < end_cmd){
					end_cmd = i;
				}
			}
		}
		if (end_cmd < tokens->size) tokens->items[end_cmd] = NULL;

		if (execv(path, tokens->items) < 0)
			perror("ext_com: failed to execute");
	}
	else {				// parent process
		if (bg)
			add_job(joblist, pid, input);
		else
			waitpid(pid, NULL, 0);
	}
}


job *new_joblist()
{
	job *joblist = calloc(JOBLIST_SIZE, sizeof(job));

	for (int i = 0; i < JOBLIST_SIZE; i++) {
		joblist[i].pid = 0;
		joblist[i].cmd = NULL;
	}

    return joblist;
}

void add_job(job *joblist, const pid_t pid, const char *cmd)
{
	for(int i = 0; i < JOBLIST_SIZE; i++) {
		if (joblist[i].pid == 0){
			joblist[i].cmd = calloc(1, strlen(cmd) + 1);
			strcpy(joblist[i].cmd, cmd);
			joblist[i].pid = pid;
			printf("[%i] %i\n", i + 1, (int) pid);
			i = JOBLIST_SIZE;
		}
	}
}

void check_jobs(job *joblist)
{
	pid_t status;
	bool zombielist[JOBLIST_SIZE];
	memset(zombielist, false, JOBLIST_SIZE*sizeof(bool) );
	int modifier = 0;	

	for (int i = 0; i < JOBLIST_SIZE; i++) {
		if (joblist[i].pid != 0) {
			status = waitpid(joblist[i].pid, NULL, WNOHANG);
			if (status == -1)
				perror("check_jobs: process does not exist");
			else if (status > 0) {
				printf("[%i]+ %s\n", i + 1, joblist[i].cmd);
				zombielist[i] = true;
			}
		}
		else
			i = JOBLIST_SIZE;
	}
	for (int k = 0; k < JOBLIST_SIZE; k++) {
		if (zombielist[k]) {
			del_job(joblist, k - modifier);
			modifier++;
		}
	}
}

void del_job(job *joblist, int index)
{
	free(joblist[index].cmd);
	for (int i = index; i < (JOBLIST_SIZE - 1); i++) {
		joblist[i].pid = joblist[i + 1].pid;
		joblist[i].cmd = joblist[i + 1].cmd;
	}
	joblist[JOBLIST_SIZE - 1].pid = 0;
	joblist[JOBLIST_SIZE - 1].cmd = NULL;
}




