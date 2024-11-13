#include <ctype.h>
#include <unistd.h>

#include "parser.h"
#include "builtins.h"
#include "help_funcs.h"
#include "jobs.h"

static char prev[3][200] = { '\0' };

int main()
{
	job *joblist = new_joblist();
	char *cwd = calloc(100, 1);
	char *path;

	while (1) {
		check_jobs(joblist);
		
		printf("%s@%s:%s> ", getenv("USER"),
							 getenv("MACHINE"),
							 getcwd(cwd, 100));

		/* input contains the whole command
		 * tokens contains substrings from input split by spaces
		 */

		char *input = get_input();
		tokenlist *tokens = get_tokens(input);

		int func = search_funcs(tokens->items[0]);

		switch (func)
		{
		case 0: // no input
			break;
		case 1: // exit
			shell_exit(prev, joblist);
			free(input);
			free(cwd);
			free_tokens(tokens);
			free(joblist);
			return 0;
		case 2: // cd
			if (tokens->items[1] && !tokens->items[2])
				shell_cd(tokens->items[1]);
			else if (tokens->items[2]){
				printf("shell: cd: too many arguments\n");
				break;
			}
			else {
				shell_cd(getenv("HOME"));
			}
			log_inputs(prev, input);
			break;
		case 3: // echo
			shell_echo(tokens);
			log_inputs(prev, input);
			break;
		case 4: // jobs
			shell_jobs(joblist);
			log_inputs(prev, input);
			break;
		case 5: // external command
			path = path_search(tokens->items[0]);
			ext_com(tokens, input, joblist, path);
			log_inputs(prev, input);
			free(path);
			break;
		default: // not found
			printf("shell: %s: command not found\n", tokens->items[0]);
			break;
		}

		free(input);
		free_tokens(tokens);
	}

	free(joblist);

	return 0;
}
