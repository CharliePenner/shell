#include "parser.h"

tokenlist *new_tokenlist(void)
{
	tokenlist *tokens = (tokenlist *) malloc(sizeof(tokenlist));
	tokens->size = 0;
	tokens->items = (char **) malloc(sizeof(char *));
	tokens->items[0] = NULL; /* make NULL terminated */
	return tokens;
}

void add_token(tokenlist *tokens, char *item)
{
	int i = tokens->size;

	tokens->items = (char **) realloc(tokens->items, (i + 2) * sizeof(char *));
	tokens->items[i] = (char *) malloc(strlen(item) + 1);
	tokens->items[i + 1] = NULL;
	strcpy(tokens->items[i], item);

	tokens->size += 1;
}

char *get_input(void)
{
	char *buffer = NULL;
	int bufsize = 0;

	char line[5];
	while (fgets(line, 5, stdin) != NULL) {
		int addby = 0;
		char *newln = strchr(line, '\n');
		if (newln != NULL)
			addby = newln - line;
		else
			addby = 5 - 1;

		buffer = (char *) realloc(buffer, bufsize + addby);
		memcpy(&buffer[bufsize], line, addby);
		bufsize += addby;

		if (newln != NULL)
			break;
	}

	buffer = (char *) realloc(buffer, bufsize + 1);
	buffer[bufsize] = 0;

	return buffer;
}

tokenlist *get_tokens(char *input)
{
	char *buf = (char *) malloc(strlen(input) + 1);
	strcpy(buf, input);

	tokenlist *tokens = new_tokenlist();

	char *tok = strtok(buf, " ");
	while (tok != NULL) {
		if (tok[0] == '$')
			add_token(tokens, env_var(tok));
		else if ((tok[0] == '~') && ((tok[1] == '/') || (tok[1] == '\0')))
			add_token(tokens, tilde_exp(tok));
		else
			add_token(tokens, tok);
		tok = strtok(NULL, " ");
	}

	free(buf);
	return tokens;
}

void free_tokens(tokenlist *tokens)
{
	for (int i = 0; i < tokens->size; i++)
		free(tokens->items[i]);
	free(tokens->items);
	free(tokens);
}

char *env_var(const char *t)
{
	char temptok[strlen(t) + 1];
	strcpy(temptok, t);
	memmove(temptok, temptok + 1, strlen(temptok));	// removes leading '$'
	char *envvar = getenv(temptok);
	if (envvar == NULL)
		return ("");
	else
		return (getenv(temptok));
}

char *tilde_exp(const char *t)
{
	char temptok[strlen(t) + 1];
	strcpy(temptok, t);
	memmove(temptok, temptok + 1, strlen(temptok));	// removes leading '~'
	char *home = getenv("HOME");
	return (strcat(home, temptok));
}
