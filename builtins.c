#include "builtins.h"

void shell_exit(char prev[3][200], job *joblist){
    
    for (int i = 0; i < JOBLIST_SIZE; i++) {
        if (joblist[i].pid == 0)
            i = JOBLIST_SIZE;
        else
            waitpid(joblist[i].pid, NULL, 0);
    }
        
    if (prev[0][0] == '\0'){
        printf("No valid commands were executed in this shell.\n");
    }
    else if (prev[2][0] == '\0'){
        printf("The last valid command executed was:\n%s\n", prev[0]);
    }
    else {
        printf("The following was the last three valid commands executed:"
                "\n1. %s\n2. %s\n3. %s\n", prev[2], prev[1], prev[0]);
    }
    
}


bool shell_cd(char* dir){
    if (chdir(dir)){
        printf("shell: cd: %s: No such file or directory\n", dir);
        return false;
    }
    return true;
}


void shell_echo(tokenlist* tokens){
    for (int i = 1; i < tokens->size && tokens->items[i][0] != '<'
                                     && tokens->items[i][0] != '>'
                                     && tokens->items[i][0] != '|'; i++){
        
        printf("%s ", tokens->items[i]);
    }
    printf("\n");
}


void shell_jobs(job *joblist){
    for (int i = 0; i < JOBLIST_SIZE; i++)
        if (joblist[i].pid != 0)
            printf("[%i]+ %i %s\n", i + 1, (int) joblist[i].pid, joblist[i].cmd);
}
