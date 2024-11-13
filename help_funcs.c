#include "help_funcs.h"

int search_funcs(char* token){
	int i = 0;
	char* tmp;
	if (token == NULL)				return i;
	i++;
	if (!strcmp(token, "exit"))		return i;
	i++;
	if (!strcmp(token, "cd"))		return i;
	i++;
	if (!strcmp(token, "echo"))		return i;
	i++;
	if (!strcmp(token, "jobs"))		return i;
	i++;
	if ((tmp = path_search(token)) != NULL){
		free(tmp);
		return i;
	}
	i++;
	return i;
}

void log_inputs(char prev[3][200], char* input){
	strcpy(prev[2], prev[1]);
	strcpy(prev[1], prev[0]);
	strcpy(prev[0], input);
}