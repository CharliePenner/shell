#include "path_search.h"

char* path_search(char* CMD){
    char* cmd = calloc(1, strlen(CMD) + 2);
    strcpy(cmd, "/");
    strcat(cmd, CMD);
    char* PATH = calloc(1, strlen(getenv("PATH")) + 1);
    memcpy(PATH, getenv("PATH"), strlen(getenv("PATH")) + 1);

    char* path = strtok(PATH, ":");
    char* buffer = calloc(1, strlen(path) + strlen(cmd) + 1);
    strcpy(buffer, path);
    strcat(buffer, cmd);

    if (!access(buffer, F_OK)){
        free(cmd);
        free(PATH);
        return buffer;
    }
    free(buffer);


    while (path){
        path = strtok(NULL, ":");
        if (path){
            buffer = calloc(1, strlen(path) + strlen(cmd) + 1);
            strcpy(buffer, path);
            strcat(buffer, cmd);
            if (!access(buffer, F_OK)){
                free(cmd);
                free(PATH);
                return buffer;
            }
            free(buffer);
        }
        else {
            free(cmd);
            free(PATH);
            return NULL;
        }
    }

    free(cmd);
    free(PATH);
    return NULL;
}
